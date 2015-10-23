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

#include "Activator.h"
#include "Clock.h"
#include <iostream>

NOSGI_SETACTIVATOR(Activator)

using namespace std;

ServiceRegistration *sreg;
Clock *c;

Activator::~Activator() {

}

void Activator::start(BundleContext &context) {
	cout << "starting Clock-Bundle..." << endl;
	c = new Clock();
	cout << "registering Service 'Clock'" << endl;
	string serviceName = "Clock";
	sreg = context.registerService(serviceName, c);
	c->tick();
}

void Activator::stop(BundleContext &context) {
	cout << "Stopping Clock-Bundle..." << endl;
	sreg->unregister();
	delete sreg;
	delete c;
}

