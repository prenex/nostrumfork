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

#include <osgi/framework/BundleException.h>
#include <nosgi/framework/BundleImpl.h>
#include "framework.h"
#include <nosgi/classloader/NOSGiClassLoader.h>
#include <nosgi/classloader/solib.h>
#include <nosgi/framework/BundleContextImpl.h>
#include <map>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

BundleImpl::BundleImpl(const string& location) throw(BundleException)
	: state(UNINSTALLED), sourcebundle(false) {
	bundleId = Framework::getNextBundleId();
	version.major = 0;
	version.minor = 0;
	version.micro = 0;
	version.qualifier = "";
	char locationtemp[256];
	string installPath(location);
	//installation destination
	snprintf(locationtemp, sizeof(locationtemp), "./nosgiwork/%016lx",
		bundleId);
	cachelocation = locationtemp;
	this->location = location;
	if (mkdir(locationtemp, 0775)) {
		cerr << "error creating directory " << cachelocation << endl;
		system(("rm -r "+cachelocation).c_str());
		throw BundleException();
	}
	//check http
	if (!location.compare(0, 7, "http://")) {
		system("rm -f ./nosgiwork/tmpbundle.zip");
		if(system(("wget -q -O ./nosgiwork/tmpbundle.zip "+location).c_str())) {
			throw BundleException();
		}
		installPath = "./nosgiwork/tmpbundle.zip";
	}
	//extract bundle to destination
	if (system(("unzip -q "+installPath+" -d "+cachelocation).c_str())) {
		system(("rm -r "+cachelocation).c_str());
		throw BundleException();
	}
	//load bundle properties out of manifest file
	parseManifest(cachelocation);
	//compile source bundle on-the-fly
	if (sourcebundle) {
		cout << "Source package detected. Compiling...";
		cout.flush();
		if (system(("make -s -C "+cachelocation).c_str())) {
			system(("rm -r "+cachelocation).c_str());
			throw BundleException();
		}
		cout << "done" << endl;
	}
	//save location
	ofstream stream((cachelocation+"/meta").c_str(),
			fstream::out|fstream::trunc);
	stream << location << endl;
	stream.close();
	//create classloader
	classLoader = new NOSGiClassLoader();
	classLoader->setClassPath(cachelocation);
	state = INSTALLED;
	BundleEvent installedEvent(BundleEvent::INSTALLED, *this);
	Framework::notifyBundleListeners(installedEvent);
	try {
		resolveAndLoad(true);
	} catch(BundleException) {
		system(("rm -r "+cachelocation).c_str());
		throw;
	}
	saveState();
}

BundleImpl::BundleImpl(long newBundleId)
	: state(UNINSTALLED), sourcebundle(false) {
	version.major = 0;
	version.minor = 0;
	version.micro = 0;
	bundleId = newBundleId;
	bool setBundleId(false);
	char locationtemp[256];
	snprintf(locationtemp, sizeof(locationtemp), "./nosgiwork/%016lx",
		bundleId);
	cachelocation = locationtemp;
	//when there is no manifest file, maybe it was deleted to mark
	//an update
	if (!parseManifest(cachelocation)) {
		switch (system(("rm -r "+cachelocation).c_str())
			|| rename((cachelocation+"_").c_str(),
				cachelocation.c_str())) {
			case 0: if (parseManifest(cachelocation)) break;
				//no break;
			case 1: cerr << "Could not read manifest-file" << endl;
				perror("");
				throw BundleException();
				break;
		}
		setBundleId = true;
	}
	//load location
	ifstream stream((cachelocation+"/meta").c_str());
	stream >> location;
	stream.close();
	//create classloader
	classLoader = new NOSGiClassLoader();
	classLoader->setClassPath(cachelocation);
	state = INSTALLED;
	BundleEvent installedEvent(BundleEvent::INSTALLED, *this);
	Framework::notifyBundleListeners(installedEvent);
	resolveAndLoad(setBundleId);
	int newstate;
	ifstream statestream((cachelocation+"/state").c_str());
	statestream >> newstate;
	statestream.close();
	if(newstate==ACTIVE) {
		start();
	}
}

BundleImpl::~BundleImpl() {
	for(unsigned int i=0; i<imports.size(); i++) {
		delete imports[i];
	}
	for(unsigned int i=0; i<exports.size(); i++) {
		delete exports[i];
	}
}

void BundleImpl::resolveAndLoad(bool setBundleId) throw(BundleException) {
	resolve();
	if (state != RESOLVED) return;
	if (setBundleId) {
		for (unsigned int i=0; i<exports.size(); i++) {
			char source[256], dest[256];
			snprintf(source, sizeof(source),
				"%s/%s-_nosgi-exported_.so",
				cachelocation.c_str(),
				exports[i]->getName().c_str());
			snprintf(dest, sizeof(dest),
				"%s/%s-%016lx.so",
				cachelocation.c_str(),
				exports[i]->getName().c_str(),
				bundleId);
			if (rename(source, dest)) perror("rename");
		}
	}
	exportPackages(setBundleId);
	if (!activatorName.empty()) {
		addImports(activatorName+".so", setBundleId);
		activator = (BundleActivator *)
			classLoader->loadActivator(activatorName);
		if (!activator) {
			throw BundleException();
		}
	}
}

void BundleImpl::addImports(const string &file, bool setBundleId) const {
	if(!setBundleId && imports.size()==0) {
		return; //no update
	}
	//file to patch
	string filename(cachelocation+"/"+file);
	//when there is an resolve-file, load it
	map<string,long> cachedImports;
	ifstream resolve((filename+".resolve").c_str(), fstream::in);
	while (!resolve.fail() && !resolve.eof()) {
		string import;
		if (resolve >> import) {
			long i;
			resolve >> i;
			cachedImports[import] = i; //TODO check version
		}
	}
	resolve.close();
	//match external imports to bundle-ids according to resolving
	bool change = false;
	for (unsigned int i=0; i<imports.size(); i++) {
		if (cachedImports.count(imports[i]->getName())==0
			||cachedImports[imports[i]->getName()]
			!=imports[i]->getBundleId()) {
			change = true;
			break;
		}
	}
	char oldSymbol[1024], newSymbol[1024];
	map<string,string> symbols;
	//do we have to renew the bundle-id?
	if (change) {
		ofstream resolveout((filename+".resolve").c_str(),
			fstream::out|fstream::trunc);
		for (unsigned int i=0; i<imports.size(); i++) {
			cerr << "patching Bundle-Id "<<bundleId<<endl;
			resolveout << imports[i]->getName() << "\t"
				<< imports[i]->getBundleId() << endl;
			snprintf(newSymbol, 256, "%s-%016lx.so",
				imports[i]->getName().c_str(),
				imports[i]->getBundleId());
			if (cachedImports.count(imports[i]->getName())>0) {
				snprintf(oldSymbol, 256, "%s-%016lx.so",
					imports[i]->getName().c_str(),
					cachedImports[imports[i]->getName()]);
				symbols[oldSymbol] = newSymbol;
			} else {
				symbols[imports[i]->getName()
					+"-_nosgi-exported_.so"] = newSymbol;
			}
		}
		resolveout.close();
	}
	//only where there is a new bundle-id
	if (setBundleId) {
		//renew local imports (bundle-id)
		for (unsigned int i=0; i<exports.size(); i++) {
			snprintf(newSymbol, 256, "%s-%016lx.so",
				exports[i]->getName().c_str(),
				bundleId);
			symbols[exports[i]->getName()+"-_nosgi-exported_.so"]
				= newSymbol;
		}
		//renew rpath for local imports /evtl. nicht nötig)
		snprintf(newSymbol, 256, "nosgiwork/%016lx", bundleId);
		symbols["_nosgi-bundle-working-dir_"] = newSymbol;
		//renew soname (bundle-id)
		snprintf(newSymbol, 256, "%s-%016lx", file.c_str(), bundleId);
		symbols[file+"-_nosgi-exported_.so"] = newSymbol;
	}
	//are there changes?
	if (change || setBundleId) {
		replaceSymbols(filename.c_str(), symbols);
	}
}

bool BundleImpl::parseManifest(const string& location) {
	string manifestpath(location+"/META-INF/MANIFEST.MF");
	fstream file;
	file.open(manifestpath.c_str(), fstream::in);
	if (!file.is_open()) {
		return false;
	}
	string line, field;
	istringstream stream;
	while (getline(file, line)) {
		stream.clear();
		stream.str(line);
		stream >> field;
		if (field == "Bundle-SymbolicName:") {
			stream >> symbolicName;
		} else if (field == "Bundle-Activator:") {
			stream >> activatorName;
		} else if (field == "Bundle-Version:") {
			version.parseVersion(stream);
		} else if (field == "Import-Package:") {
			string newImport;
			char c;
			while (stream >> newImport && !newImport.empty()) {
				if(newImport.at(newImport.size()-1) == ';') {
					newImport.erase(newImport.size()-1);
					while(stream >> c && c != '=');
					imports.push_back(
						new NeededPackage(newImport,
						new VersionRange(stream)));
				} else {
					imports.push_back(
						new NeededPackage(newImport,
							NULL));
				}
			}
		} else if (field == "Export-Package:") {
			string newExport;
			stream >> newExport;
			if (newExport.at(newExport.size()-1) == ';') {
				newExport.erase(newExport.size()-1);
				char c;
				while(stream >> c && c != '=');
				Version version;
				version.parseVersion(stream);
				exports.push_back(new Package(newExport,version));
			} else {
				exports.push_back(new Package(newExport));
			}
		} else if (field == "Bundle-Content:") {
			string content;
			stream >> content;
			if (content == "source") {
				sourcebundle = true;
			}
		}
	}
	file.close();
	return true;
}

/*string Bundle::getDirName(const string &filename) {
	FILE *f;
	f = popen(("unzip -p "+filename+" META-INF/MANIFEST.MF").c_str(), "r");
	char buf[1024];
	bool found = false;
	string sym, ver("1.0.0");
	while(fgets(buf, sizeof(buf), f)) {
		if (!strncmp(buf, "Bundle-SymbolicName:", 20)) {
			for (int n=strlen(buf)-1; buf[n]=='\n' || buf[n]=='\r';
					n--) {
				buf[n] = '\0';
			}
			sym = buf+21;
			if (found) {
				fclose(f);
				return sym+"-"+ver;
			}
			found = true;
			continue;
		}
		if (!strncmp(buf, "Bundle-Version:", 15)) {
			for (int n=strlen(buf)-1; buf[n]=='\n' || buf[n]=='\r';
					n--) {
				buf[n] = '\0';
			}
			ver = buf+16;
			if (found) {
				fclose(f);
				return sym+"-"+ver;
			}
			found = true;
		}
	}
	fclose(f);
	if (!sym.empty()) {
		return sym+"-"+ver;
	}
	return "";
}*/

long BundleImpl::getBundleId() const {
	return bundleId;
}

string &BundleImpl::getLocation() {
	return location;
}

string &BundleImpl::getSymbolicName() {
	return symbolicName;
}

string BundleImpl::getVersionString() {
	stringstream stream;
	stream << version.major << "." << version.minor 
		<< "." << version.micro;
	if (version.qualifier != "") stream << "." << version.qualifier;
	string version;
	stream >> version;
	return version;
}

int BundleImpl::getState() const {
	return state;
}

Version *BundleImpl::doResolve(Package *package, NeededPackage *neededPackage,
		Version *version, long bid) {
	VersionRange *range = neededPackage->getVersionRange();
	if (range && !range->isIncluded(package->getVersion())) {
		return version;
	}
	//accept only new versions
	if (version && !(*version<package->getVersion())) {
		return version;
	}
	neededPackage->setBundleId(bid);
	if (neededPackage->getPackage()) {
		neededPackage->getPackage()->decImports();
	}
	package->incImports();
	neededPackage->setPackage(package);
	cerr << " " << bid << " resolved!" << endl;
	return &package->getVersion();
}

void BundleImpl::resolve() {
	multimap<string,Bundle *> *bundles = Framework::getBundles();
	Package *package;
	map<int,Bundle *> resolvedBundles;
	for(unsigned int i=0; i<imports.size(); i++) {
		Version *version(NULL);
		string packageName(imports[i]->getName());
		cerr << "Resolve import " << packageName << ", found Bundles:";
		//test already resolved bundles
		for(map<int,Bundle *>::iterator it = resolvedBundles.begin();
				it != resolvedBundles.end(); it++) {
			if((package=it->second->isExport(packageName))) {
				version = doResolve(package,imports[i],version,
						it->second->getBundleId());
			}
		}
		//go only ahead, when not resolved
		if (version) continue;
		//test not resolved bundles
		for(multimap<string,Bundle *>::iterator it = bundles->begin();
				it != bundles->end(); it++) {
			//skip resolved bundles
			if (resolvedBundles.count(it->second->getBundleId())) {
				continue;
			}
			if ((package=it->second->isExport(packageName))) {
				version = doResolve(package,imports[i],version,
						it->second->getBundleId());
			}
		}
		cerr << endl;
		if (!version) {
			cerr << "Could not resolve " << imports[i]->getName()
				<< endl;
			return;
		}
	}
	state = RESOLVED;
	BundleEvent resolveEvent(BundleEvent::RESOLVED, *this);
	Framework::notifyBundleListeners(resolveEvent);
}

void BundleImpl::start() throw(BundleException) {
	if (state == ACTIVE) return;
	if (state != RESOLVED) throw BundleException();
	if (activator) {
		BundleEvent startingEvent(BundleEvent::STARTING, *this);
		Framework::notifyBundleListeners(startingEvent);
		state = STARTING;
		context = new BundleContextImpl();
		activator->start(*context);
		state = ACTIVE;
		saveState();
		BundleEvent startedEvent(BundleEvent::STARTED, *this);
		Framework::notifyBundleListeners(startedEvent);
	} else {
		cerr << "bundle has no activator" << endl;
	}
}

void BundleImpl::stop() {
	if (state == RESOLVED) return;
	state = STOPPING;
	BundleEvent stoppingEvent(BundleEvent::STOPPING, *this);
	Framework::notifyBundleListeners(stoppingEvent);
	activator->stop(*context);
	delete context;
	context = NULL;
	state = RESOLVED;
	saveState();
	BundleEvent resolvedEvent(BundleEvent::STOPPED, *this);
	Framework::notifyBundleListeners(resolvedEvent);
}

void BundleImpl::doUninstall() {
	if (state == ACTIVE) stop();
	if (state == RESOLVED) {
		delete activator;
		activator = 0;
		classLoader->releaseActivator(activatorName);
		unExportPackages();
		delete classLoader;
	}
	//release imports
	for (unsigned int i = 0; i<imports.size(); i++) {
		if (imports[i]->getPackage()) {
			imports[i]->getPackage()->decImports();
		}
	}
}

void BundleImpl::saveState() {
	ofstream stream((cachelocation+"/state").c_str(),
		fstream::out|fstream::trunc);
	stream << state << endl;
	stream.close();
}

void BundleImpl::uninstall() throw(BundleException) {
	for (unsigned int i=0; i<exports.size(); i++) {
		if (exports[i]->isImported()) {
			cerr << "Bundle is in use\n" << endl;
			throw BundleException();
		}
	}
	doUninstall();
	Framework::uninstallBundle(bundleId);
	state = UNINSTALLED;
	BundleEvent uninstalledEvent(BundleEvent::UNINSTALLED, *this);
	Framework::notifyBundleListeners(uninstalledEvent);
}

extern void *doUpdate(void *);
pthread_cond_t updatedcond = PTHREAD_COND_INITIALIZER;

void BundleImpl::update() throw(BundleException) {
	if (state == UNINSTALLED) {
		throw BundleException();//("Cannot update uninstalled bundle");
	}

	//start update in an own thread, that bundles can update itself
	pthread_t tid;
	pthread_create(&tid, NULL, ::doUpdate, (void *)this);
	pthread_detach(tid);
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_wait(&updatedcond, &mutex);
}

void *doUpdate(void *attr) {
	((BundleImpl *)attr)->doUpdate();
	return 0;
}

void BundleImpl::doUpdate() {
	int oldState = state;
	if (state == ACTIVE) {
		stop();
	}

	bool imported = false;
	for (unsigned int i=0; i<exports.size(); i++) {
		if (exports[i]->isImported()) {
			imported = true;
			break;
		}
	}

	string installlocation(cachelocation);
	if (!imported) {
		doUninstall();
	} else {
		//delete manifest-file to signal an update
		if (unlink((cachelocation+"/META-INF/MANIFEST.MF").c_str())<0) {
			perror("unlink");
		}
		installlocation += "_";
	}
	if (system(("rm -rf "+installlocation+"/*").c_str())) {
		cerr << "error deleting old bundle files" << endl;
	}
	//extract new bundle version to destination
	if (system(("unzip -q "+location+" -d "+installlocation).c_str())) {
		system(("rm -r "+installlocation).c_str());
		throw BundleException();
	}
	if (imported) {
		//read source bundle header out of manifest file
		string manifestpath(installlocation+"/META-INF/MANIFEST.MF");
		fstream file;
		file.open(manifestpath.data(), fstream::in);
		if (!file.is_open()) {
			cerr << "could not open Manifest-File" << endl;
			throw BundleException();
		}
		string line, field;
		istringstream stream;
		while (getline(file, line)) {
			stream.clear();
			stream.str(line);
			stream >> field;
			if (field == "Bundle-Content:") {
				string content;
				stream >> content;
				if (content == "source") {
					sourcebundle = true;
				}
			}
		}
		file.close();
	} else { //!imported
		//read bundle properties out of manifestfile
		parseManifest(installlocation);
	}
	//compile source bundle on-the-fly
	if (sourcebundle) {
		cout << "Source package detected. Compiling...";
		cout.flush();
		if (system(("make -s -C "+installlocation).c_str())) {
			system(("rm -r "+installlocation).c_str());
			throw BundleException();
		}
		cout << "done" << endl;
	}
	if (!imported) {
		//save location
		ofstream stream((installlocation+"/meta").c_str(),
				fstream::out|fstream::trunc);
		stream << location << endl;
		stream.close();
		//create classloader
		classLoader = new NOSGiClassLoader();
		classLoader->setClassPath(installlocation.c_str());
		resolveAndLoad(true);
	}

	BundleEvent updateEvent(BundleEvent::UPDATED, *this);
	Framework::notifyBundleListeners(updateEvent);

	if (oldState == ACTIVE) {
		start();
	}
	pthread_cond_signal(&updatedcond);
}

Package *BundleImpl::isExport(string &package) const {
	for(unsigned int i=0; i<exports.size(); i++) {
		if(exports[i]->getName() == package) {
			return exports[i];
		}
	}
	return NULL;
}

void BundleImpl::exportPackages(bool setBundleId) const {
	char prefix[17];
	snprintf(prefix, sizeof(prefix), "%016lx", bundleId);
	for(unsigned int i=0; i<exports.size(); i++) {
		addImports(exports[i]->getName() + "-" + prefix + ".so",
				setBundleId);
	}
	//two step, so that dlopen() do not already load dependencies,
	//that are overrided at the filesystem later on
	for(unsigned int i=0; i<exports.size(); i++) {
		char filename[256];
		snprintf(filename, sizeof(filename), "%s/%s-%016lx.so",
			location.c_str(),
			exports[i]->getName().c_str(),
			bundleId);
		cerr << "exporting " << exports[i]->getName() << " Version: "
		       << exports[i]->getVersion().getVersionString() << endl;
		classLoader->loadSo(exports[i]->getName()+"-"+prefix+".so");
	}
}

void BundleImpl::unExportPackages() {
	for(unsigned int i=0; i<exports.size(); i++) {
		delete exports[i];
	}
	classLoader->releaseAllSo();
	exports.clear();
}
