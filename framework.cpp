/*
 * nOSGi Project - http://www-vs.informatik.uni-ulm.de/proj/nosgi/
 *
 * Copyright (C) 2009-2011 Aspectix Research Team,
 * Institute of Distributed Systems, Ulm University, Germany
 *
 * This file is part of the nOSGi Project, which is released
 * under the terms of the GNU General Public License.
 * See the LICENSE.txt file in the root directory of the software
 * distribution for more details.
 */

#include "framework.h"
#include <iostream>
#include <errno.h>
#include <fstream>
#include <pthread.h>
#include <map>
#include <sys/stat.h>
#include <osgi/framework/Bundle.h>
#include <osgi/framework/BundleException.h>
#include <nosgi/framework/BundleContextImpl.h>
#include <nosgi/framework/FrameworkEvent.h>
#include <nosgi/framework/ServiceReference.h>
#include <nosgi/framework/SynchronousBundleListener.h>

using namespace std;

static map<long,Bundle *> bundleIds;
static multimap<string,Bundle *> bundles;
static bool preloading = true;
static bool saveOnExit = true;
static pthread_cond_t shutdownSignal = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Global pointer to the framework - useful for the stop operation
static Framework *framework = nullptr;

//private framework members
set<FrameworkListener *> Framework::frameworkListeners;
set<BundleListener *> Framework::syncBundleListeners;
map<ServiceListener *, Filter *> Framework::serviceListeners;
map<long,void *> Framework::serviceRegistry;
multimap<string,ServiceReference *> Framework::interfaceRegistry;
long Framework::fid = 0;
long Framework::bid = 1;

string frameworkLocation = "framework location";
string frameworkName = "nOSGi framework";
string versionString = "0.6.0";

BundleContextImpl context;

void Framework::addFrameworkListener(FrameworkListener *listener) {
	frameworkListeners.insert(listener);
}

void Framework::notifyFrameworkListeners(FrameworkEvent &event) {
	if (frameworkListeners.empty()) {
		return;
	}
	for (set<FrameworkListener *>::iterator it = frameworkListeners.begin();
			it != frameworkListeners.end(); it++) {
		(*it)->frameworkEvent(event);
	}
}

void Framework::removeFrameworkListener(FrameworkListener *listener) {
	frameworkListeners.erase(listener);
}


void Framework::addBundleListener(BundleListener *listener) {
	if (dynamic_cast<SynchronousBundleListener *>(listener)) {
		syncBundleListeners.insert(listener);
	} else {
		cerr << "uninplemented!" << endl;
	}
}

void Framework::notifyBundleListeners(BundleEvent &event) {
	if (syncBundleListeners.empty()) {
		return;
	}
	for (set<BundleListener *>::iterator it = syncBundleListeners.begin();
			it != syncBundleListeners.end(); it++) {
		(*it)->bundleChanged(event);
	}
}

void Framework::removeBundleListener(BundleListener *listener) {
	syncBundleListeners.erase(listener);
}

void Framework::addServiceListener(ServiceListener *listener, Filter *filter) {
	serviceListeners[listener] = filter;
}

void Framework::notifyServiceListeners(ServiceEvent &event) {
	if (serviceListeners.empty()) {
		return;
	}
	for (map<ServiceListener*, Filter*>::iterator it = serviceListeners.begin();
			it != serviceListeners.end(); it++) {
		if (!it->second || it->second->match(
				event.getServiceReference().properties)) {
			it->first->serviceChanged(event);
		}
	}
}

void Framework::removeServiceListener(ServiceListener *listener) {
	map<ServiceListener*,Filter*>::iterator it(serviceListeners.find(listener));
	delete it->second;
	serviceListeners.erase(it);
}

long Framework::getNextBundleId() {
	return bid++;
}

long Framework::getNextServiceId() {
	return fid++;
}

Bundle *Framework::installBundle(Bundle *bundle) {
	string &symbolicName = bundle->getSymbolicName();
	//add bundle to loadedbundle to remember for next startup
	if (!preloading) {
		ofstream loadedBundles;
		loadedBundles.open("./nosgiwork/loadedbundles.conf", ios::app);
		loadedBundles << bid-1 << "\t" << symbolicName << endl;
		loadedBundles.close();
	}
	bundleIds[bundle->getBundleId()] = bundle;
	bundles.insert(pair<string,Bundle *>(symbolicName,bundle));
	return bundle;
}

Bundle *Framework::getBundle(long id) {
	return bundleIds.count(id)?bundleIds[id]:NULL;
}

vector<Bundle *> *Framework::getInstalledBundles() {
	vector<Bundle *> *bundles = new vector<Bundle *>(bundleIds.size());
	int i = 0;
	for (map<long,Bundle *>::iterator it = bundleIds.begin();
			it!=bundleIds.end(); it++) {
		bundles->at(i++) = it->second;
	}
	return bundles;
}

multimap<string,Bundle *> *Framework::getBundles() {
	return &bundles;
}

void Framework::uninstallBundle(long id) {
	for (multimap<string,Bundle *>::iterator it
		= bundles.find(bundleIds[id]->getSymbolicName());
		it != bundles.upper_bound(bundleIds[id]->getSymbolicName());
		it++) {
		if (it->second->getBundleId() == id) {
			bundles.erase(it);
			break;
		}
	}
	bundleIds.erase(id);
	char command[256];
	snprintf(command, sizeof(command), "rm -r ./nosgiwork/%016lx/", id);
	if (system(command)) perror("system");
	//write out file for installed bundles at exit
	saveOnExit = true;
}

void Framework::addServiceRegistration(long id, void *service) {
	serviceRegistry[id] = service;
}

void *Framework::getService(const ServiceReference *ref) {
	return serviceRegistry.count(ref->getId()) ?
		serviceRegistry[ref->getId()] : NULL;
}

void Framework::printServices() {
	for (multimap<string,ServiceReference *>::iterator it
		= interfaceRegistry.begin(); it!=interfaceRegistry.end();it++) {
		cout << it->second->getId() << " " << it->first << endl;
	}
}

void printServices() {
	Framework::printServices();
}

void Framework::removeServiceRegistration(long id) {
	serviceRegistry.erase(id);
}

void Framework::addInterfaceRegistration(ServiceReference *ref,
		const string &interface) {
	interfaceRegistry.insert(
			pair<string,ServiceReference *>(interface, ref));
}

ServiceReference *Framework::getServiceReference(
		const string &interfaceName) {
	return interfaceRegistry.count(interfaceName) ?
		interfaceRegistry.find(interfaceName)->second : NULL;
}

void Framework::removeInterfaceRegistration(long id, const string &interface) {
	for (multimap<string,ServiceReference *>::iterator it
		= interfaceRegistry.find(interface);
		it != interfaceRegistry.upper_bound(interface); it++) {
		if (it->second->getId() == id) {
			interfaceRegistry.erase(it);
			break;
		}
	}
}

void Framework::preload() {
	ifstream loadedBundles("./nosgiwork/loadedbundles.conf");
	if (!loadedBundles.fail()) {
		while (!loadedBundles.eof()) {
			string bundleName;
			loadedBundles >> bid;
			loadedBundles >> bundleName;
			if (!bundleName.empty()) {
				try {
					installBundle(new BundleImpl(bid++));
				} catch(BundleException) {
					cerr << "could not load bundle " 
						<< bid-1 << ": " << bundleName
						<< endl;
				}
			}
		}
	}
	loadedBundles.close();
	preloading = false;
}

void Framework::startup() {
	{
		FrameworkEvent event(FrameworkEvent::STARTED, NULL, NULL);
		notifyFrameworkListeners(event);
	}
	//framework thread waits here until framework shuts down
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&shutdownSignal, &mutex);
}

void Framework::shutdown() {
	if (saveOnExit) {
		ofstream loadedBundlesOutput("./nosgiwork/loadedbundles.conf",
				ios::trunc);
		for (map<long, Bundle*>::iterator it = bundleIds.begin();
				it != bundleIds.end(); it++) {
			if (it->first == 0) continue;
			loadedBundlesOutput << it->first << "\t"
				<< it->second->getSymbolicName() << endl;
		}
		loadedBundlesOutput.close();
	}
}

void shutdownNOSGiFramework() {
	framework->stop();
}

long Framework::getBundleId() const {
	return 0;
}

string &Framework::getLocation() {
	return frameworkLocation;
}

string &Framework::getSymbolicName() {
	return frameworkName;
}

string Framework::getVersionString() {
	return versionString;
}

int Framework::getState() const {
	return ACTIVE;
}

void Framework::start() throw(BundleException) {

}

void Framework::stop() {
	pthread_cond_signal(&shutdownSignal);
}

void Framework::update() throw(BundleException) {

}

void Framework::uninstall() throw(BundleException) {
	throw BundleException();
}

Package *Framework::isExport(string &package) const {
	return NULL;
}

int main() {
	struct stat statbuf;
	framework = new Framework();
	Framework::installBundle(framework); //0 is framework
	if (stat("./nosgiwork/", &statbuf)) { //first start
		if (errno != ENOENT) {
			perror("stat");
			return 1;
		}
		string path;
		cout << "First start. Please enter path to console.zip: ";
		getline(cin, path);
		if (mkdir("./nosgiwork/", 0775)) {
			perror("mkdir");
			return 1;
		}
		try {
			Bundle *b = Framework::installBundle(
				new BundleImpl(path));
			b->start();
		} catch(BundleException) {
			system("rm -r ./nosgiwork/");
			cerr << "Could not start console" << endl;
			return 1;
		}
		preloading = false;
	} else {
		Framework::preload(); //load old bundles
	}
	Framework::startup();
	Framework::shutdown();
	return 0;
}
