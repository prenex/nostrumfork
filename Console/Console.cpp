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

#include "Console.h"
#include "Clock.h"
#include <nosgi/framework/Bundle.h>
#include <nosgi/framework/ServiceReference.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

extern void shutdownNOSGiFramework();
pthread_t Console::tid;

Console::Console(BundleContext &newContext)
	: running(false), context(newContext) {

}

void Console::run() {
	assert(!running);
	running = true;
	pthread_create(&tid, 0, &Console::starter, this);
}

void Console::stop() {
	assert(running);
	running = false;
	pthread_cancel(tid);
	pthread_join(tid, NULL);
}

void *Console::starter(void *instance) {
	((Console *)instance)->loop();
	return 0;
}

string Console::getStateString(int state) {
	switch (state) {
		case Bundle::UNINSTALLED: return "UNINSTALLED";
		case Bundle::INSTALLED: return "INSTALLED";
		case Bundle::RESOLVED: return "RESOLVED";
		case Bundle::STARTING: return "STARTING";
		case Bundle::STOPPING: return "STOPPING";
		case Bundle::ACTIVE: return "ACTIVE  ";
	}
	return "ERROR   ";
}

void Console::loop() {
	string cmdline, cmd;
	istringstream stream;
	while (cout << "nosgi> " && getline(cin, cmdline)) {
		cmd.clear();
		stream.clear();
		stream.str(cmdline);
		stream >> cmd;
		if (cmd == "help") {
			cout << "\n" \
				"install <bundle>\tloads bundle <bundle>\n" \
				"uninstall <bundle>\tunloads bundle <bundle>\n"\
				"start <bundle>\t\tstarts bundle <bundle>\n"\
				"stop <bundle>\t\tstops bundle <bundle>\n"\
				"update <bundle>\t\tupdates bundle <bundle>\n"\
				"ss\t\t\tshows the installed bundles\n"\
				"services\t\tshows the installed services\n"\
				"exit\t\t\tshutdowns framework\n"\
				"help\t\t\tprints this screen\n\n";
		} else if (cmd == "ss") {
			vector<Bundle *> *bundles = context.getBundles();
			cout << endl << "Id\tState\t\tBundle" << endl;
			for (vector<Bundle *>::iterator it = bundles->begin();
					it != bundles->end(); it++) {
				Bundle *bundle = (*it);
				cout << bundle->getBundleId() << "\t"
					<< getStateString(bundle->getState())
					<< "\t"
					<< bundle->getSymbolicName()
					<< " (Version: "
					<< bundle->getVersionString() << ")"
					<< endl;
			}
	       		cout << endl;
			delete bundles;
		} else if (cmd == "services") {
			extern void printServices(); //TODO
			printServices();
			cout << endl;
		} else if (cmd == "install") {
			string bundlePath;
			if (!(stream >> bundlePath)) {
				cerr << "usage: install <bundle-path>" << endl;
				continue;
			}
			try {
				context.installBundle(bundlePath);
			} catch (BundleException &e) {
				cerr << "Could not install bundle" << endl;
			}
		} else if (cmd == "uninstall") {
			long bundleId;
			Bundle *bundle;
			if (!(stream >> bundleId)) {
				cerr << "usage: uninstall <bundle-id>" << endl;
				continue;
			}
			if (!(bundle = context.getBundle(bundleId))) {
				cerr << "Bundle not found" << endl;
				continue;
			}
			try {
				bundle->uninstall();
				delete bundle;
			} catch(BundleException &e) {
				cerr << "Could not uninstall bundle" << endl;
			}
		} else if (cmd == "start") {
			long bundleId;
			Bundle *bundle;
			if (!(stream >> bundleId)) {
				cerr << "usage: start <bundle-id>" << endl;
				continue;
			}
			if (!(bundle = context.getBundle(bundleId))) {
				cerr << "Bundle not found" << endl;
				continue;
			}
			bundle->start();
		} else if (cmd == "stop") {
			long bundleId;
			Bundle *bundle;
			if (!(stream >> bundleId)) {
				cerr << "usage: stop <bundle-id>" << endl;
				continue;
			}
			if (!(bundle = context.getBundle(bundleId))) {
				cerr << "Bundle not found" << endl;
				continue;
			}
			bundle->stop();
		} else if (cmd == "update") {
			long bundleId;
			Bundle *bundle;
			if (!(stream >> bundleId)) {
				cerr << "usage: update <bundle-id>" << endl;
				continue;
			}
			if (!(bundle = context.getBundle(bundleId))) {
				cerr << "Bundle not found" << endl;
				continue;
			}
			try {
				bundle->update();
			} catch (BundleException &e) {
				cerr << "Cannot update bundle" << endl;
			}
		} else if (cmd == "time") {
			ServiceReference *ref =
				context.getServiceReference("Clock");
			if (!ref) {
				cout << "No suitable service found" << endl;
				continue;
			}
			Clock *clock = (Clock*)context.getService(ref);
			clock->tick();
		} else if (cmd == "exit") {
			shutdownNOSGiFramework();
			return;
		}
	}
}
