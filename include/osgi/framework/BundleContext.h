/**
 * @file BundleContext.h
 * 
 * @brief Declaration of the BundleContext interface.
 * 
 * @section LICENSE
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

#ifndef BUNDLECONTEXT_H
#define BUNDLECONTEXT_H

#include <osgi/framework/Bundle.h>
#include <osgi/framework/BundleException.h>
#include <nosgi/framework/BundleListener.h>
#include <nosgi/framework/Property.h>
#include <nosgi/framework/ServiceListener.h>
#include <nosgi/framework/ServiceReference.h>
#include <nosgi/framework/ServiceRegistration.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

/**
 * @brief Interface class for the bundle's execution context within the Framework.
 * 
 * The bundle context is created for a bundle when the bundle is started and
 * it provides means for bundle's communication with the OSGi framework.
 */
class BundleContext {
public:
	/*
	 * Destructor
	 */
	virtual ~BundleContext() {};


	/**
	 * @brief Registers the service object under the specified name into the Framework.
	 * 
	 * @param interfaceName String representing the name under which the service
	 *                      object is registered.
	 * @param service Pointer to the service object registered.
	 * 
	 * @return Pointer to a service registration object.
	 */
	virtual ServiceRegistration *registerService(string &interfaceName,
		void *service) = 0;

	/**
	 * @brief Registers the service object under the specified name into the Framework.
	 * 
	 * The service object is registered with the specified properties under the specified name.
	 * 
	 * @param interfaceName String representing the name under which the service
	 *                      object is registered.
	 * @param service Pointer to the service object registered.
	 * @param properties
	 * 
	 * @return Pointer to a service registration object.
	 */
	virtual ServiceRegistration *registerService(string &interfaceName,
		void *service, map<string,Property*> *properties) = 0;

	/**
	 * @brief Adds the specified BundleListener object to the bundle context.
	 * 
	 * The BundleListener object is notified when a bundle lifecycle state changes. 
	 * The listener is added to the list of bundle listeners registered in
	 * the bundle context if it is not already present in the list.
	 * 
	 * @param listener Pointer to the object implementing the BundleListener
	 *                 interface to be added.
	 */
	virtual void addBundleListener(BundleListener *listener) = 0;

	/**
	 * @brief Removes the BundleListener object from the bundle context.
	 * 
	 * The listener is removed from the list of bundle listeners registered in
	 * the bundle context.
	 * 
	 * @param listener Pointer to the BundleListener object to be removed.
	 */
	virtual void removeBundleListener(BundleListener *listener) = 0;

	/**
	 * @brief Adds the specified ServiceListener object to the bundle context.
	 * 
	 * The ServiceListener object is notified when a service lifecycle state changes. 
	 * The listener is added to the list of service listeners registered in
	 * the bundle context if it is not already present in the list.
	 * 
	 * @param listener Pointer to the object implementing the ServiceListener
	 *                 interface to be added.
	 */
	virtual void addServiceListener(ServiceListener *listener) = 0;

	/**
	 * @brief Adds the specified ServiceListener object with the specified filter to the bundle context.
	 * 
	 * The ServiceListener object is notified when a service lifecycle state changes
	 * and the change meets the criteria specified by the filter.
	 * The listener is added to the list of service listeners registered in
	 * the bundle context if it is not already present in the list.
	 * 
	 * @param listener Pointer to the object implementing the ServiceListener
	 *                 interface to be added.
	 * @param filter   String defining the filter criteria.
	 * 
	 * @see Filter
	 */
	virtual void addServiceListener(ServiceListener *listener, string filter) = 0;

	/**
	 * @brief Removes the ServiceListener object from the bundle context.
	 * 
	 * The listener is removed from the list of service listeners registered in
	 * the bundle context.
	 * 
	 * @param listener Pointer to the ServiceListener object to be removed.
	 */
	virtual void removeServiceListener(ServiceListener *listener) = 0;

	/**
	 * @brief Returns the bundle with the specified identifier. 
	 * 
	 * @param id The identifier of the bundle to retrieve.
	 * 
	 * @return Pointer to the bundle object or NULL if the identifier does not
	 *         match any installed bundle.
	 */
	virtual Bundle *getBundle(long id) const = 0;

	/**
	 * @brief Returns a list of all installed bundles. 
	 * 
	 * @return Pointer to the list of installed bundles.
	 */
	virtual vector<Bundle *> *getBundles() const = 0;

	/**
	 * @brief Returns the service object referenced by the specified ServiceReference object.
	 * 
	 * @param ref Pointer to the reference object of the service. 
	 * 
	 * @return Pointer to the service object associated with the ServiceReference object.
	 */
	virtual void *getService(const ServiceReference *ref) const = 0;

	/**
	 * @brief Returns a ServiceReference object for a service that was registered under the name.
	 * 
	 * @param interfaceName String representing the name under which the service
	 *                      object is registered.
	 * 
	 * @return Pointer to the ServiceReference object or NULL if no services
	 *         are registered under the name.
	 */
	virtual ServiceReference *getServiceReference(
		const string &interfaceName) const = 0;

	/**
	 * @brief Installs bundle from a filesystem location.
	 * 
	 * @param location String representing the bundle location.
	 * 
	 * @return Pointer to the bundle object of the installed bundle or NULL
	 *         if the installation fails.
	 * 
	 * @throw BundleException Thrown if the installation failed.
	 */
	virtual Bundle *installBundle(const string& location) const
		throw(BundleException) = 0;
};

#endif
