/**
 * @file BundleContextImpl.h
 * 
 * @brief Declaration of the BundleContextImpl class.
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

#ifndef BUNDLECONTEXTIMPL_H
#define BUNDLECONTEXTIMPL_H

#include <osgi/framework/BundleContext.h>

/**
 * @brief Implementation class of the BundleContext interface.
 */
class BundleContextImpl : public BundleContext {
public:

	/**
	 * Constructor
	 */
	BundleContextImpl();

	/**
	 * Destructor
	 */
	virtual ~BundleContextImpl();

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
		void *service);

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
		void *service, map<string,Property*> *properties);

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
	virtual void addBundleListener(BundleListener *listener);

	/**
	 * @brief Removes the BundleListener object from the bundle context.
	 * 
	 * The listener is removed from the list of bundle listeners registered in
	 * the bundle context.
	 * 
	 * @param listener Pointer to the BundleListener object to be removed.
	 */
	virtual void removeBundleListener(BundleListener *listener);

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
	virtual void addServiceListener(ServiceListener *listener);

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
	virtual void addServiceListener(ServiceListener *listener, string filter);

	/**
	 * @brief Removes the ServiceListener object from the bundle context.
	 * 
	 * The listener is removed from the list of service listeners registered in
	 * the bundle context.
	 * 
	 * @param listener Pointer to the ServiceListener object to be removed.
	 */
	virtual void removeServiceListener(ServiceListener *listener);

	/**
	 * @brief Returns the bundle with the specified identifier. 
	 * 
	 * @param id The identifier of the bundle to retrieve.
	 * 
	 * @return Pointer to the bundle object or NULL if the identifier does not
	 *         match any installed bundle.
	 */
	virtual Bundle *getBundle(long id) const;

	/**
	 * @brief Returns a list of all installed bundles. 
	 * 
	 * @return Pointer to the list of installed bundles.
	 */
	virtual vector<Bundle *> *getBundles() const;

	/**
	 * @brief Returns the service object referenced by the specified ServiceReference object.
	 * 
	 * @param ref Pointer to the reference object of the service. 
	 * 
	 * @return Pointer to the service object associated with the ServiceReference object.
	 */
	virtual void *getService(const ServiceReference *ref) const;

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
		const string &interfaceName) const;

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
		throw(BundleException);

	/**
	 * @brief just for debugging purposes to print out installed bundles
	 */
	void printBundles();

	/**
	 * @brief just for debugging purpoeses to print out available services
	 */
	void printServices() const;
};

#endif
