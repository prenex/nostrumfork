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
#include <osgi/framework/Bundle.h>
#include <nosgi/framework/BundleContextImpl.h>
#include <nosgi/framework/Property.h>
#include <nosgi/framework/RFC1960Filter.h>
#include <nosgi/framework/ServiceReference.h>
#include <nosgi/framework/ServiceRegistration.h>
#include <map>
#include <string>
#include <iostream>

using namespace std;

BundleContextImpl::BundleContextImpl() {
}

BundleContextImpl::~BundleContextImpl() {
}

void BundleContextImpl::addBundleListener(BundleListener *listener) {
	Framework::addBundleListener(listener);
}

void BundleContextImpl::removeBundleListener(BundleListener *listener) {
	Framework::removeBundleListener(listener);
}

void BundleContextImpl::addServiceListener(ServiceListener *listener) {
	Framework::addServiceListener(listener, NULL);
}

void BundleContextImpl::addServiceListener(ServiceListener *listener, string filter) {
	Framework::addServiceListener(listener, RFC1960Filter::fromString(filter));
}

void BundleContextImpl::removeServiceListener(ServiceListener *listener) {
	Framework::removeServiceListener(listener);
}

ServiceRegistration *BundleContextImpl::registerService(string &interfaceName,
	void *service) {
	return registerService(interfaceName, service, NULL);
}

ServiceRegistration *BundleContextImpl::registerService(string &interfaceName,
	void *service, map<string,Property*> *props) {
	ServiceRegistration *reg = new ServiceRegistration(interfaceName);
	ServiceReference *ref = reg->getReference();
	ref->properties = props;
	Framework::addInterfaceRegistration(ref, interfaceName);
	Framework::addServiceRegistration(ref->getId(), service);
	ServiceEvent event(ServiceEvent::REGISTERED, *ref);
	Framework::notifyServiceListeners(event);
	return reg;
}

Bundle *BundleContextImpl::installBundle(const string &location) const
	throw(BundleException) {
	return Framework::installBundle(new BundleImpl(location));
}

Bundle *BundleContextImpl::getBundle(long id) const {
	return Framework::getBundle(id);
}

vector<Bundle *> *BundleContextImpl::getBundles() const {
	return Framework::getInstalledBundles();
}

ServiceReference *BundleContextImpl::getServiceReference(
	const string &interfaceName) const {
	return Framework::getServiceReference(interfaceName);
}

void *BundleContextImpl::getService(const ServiceReference *ref) const {
	return Framework::getService(ref);
}

/*string BundleContext::getStateString(int state) {
	switch (state) {
		case Bundle::UNINSTALLED: return "UNINSTALLED";
		case Bundle::INSTALLED: return "INSTALLED";
		case Bundle::RESOLVED: return "RESOLVED";
		case Bundle::STARTING: return "STARTING";
		case Bundle::STOPPING: return "STOPPING";
		case Bundle::ACTIVE: return "ACTIVE  ";
	}
	return "ERROR   ";
}*/
