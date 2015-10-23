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
#include <nosgi/framework/ServiceEvent.h>
#include <nosgi/framework/ServiceRegistration.h>
#include <nosgi/framework/ServiceReference.h>
#include <map>
#include <string>

using namespace std;

ServiceRegistration::ServiceRegistration(string& interf) :
		ranking(0) {
	id = Framework::getNextServiceId();
	reference = new ServiceReference(this);
	interface = interf;
}

ServiceRegistration::~ServiceRegistration() {
	delete reference;
}

void ServiceRegistration::unregister() {
	Framework::removeInterfaceRegistration(id, interface);
	Framework::removeServiceRegistration(id);
	ServiceEvent event(ServiceEvent::UNREGISTERING, *reference);
	Framework::notifyServiceListeners(event);
}

long ServiceRegistration::getId() const {
	return id;
}

int ServiceRegistration::getRanking() const {
	return ranking;
}

ServiceReference *ServiceRegistration::getReference() const {
	return reference;
}

void ServiceRegistration::setProperties(map<string,Property*> *props) {
	//FIXME delete properties
	if (reference->properties) delete reference->properties;
	reference->properties = props;
	ServiceEvent event(ServiceEvent::MODIFIED, *reference);
	Framework::notifyServiceListeners(event);
}
