/**
 * @file framework.h
 * 
 * @brief Declaration of the Framework class.
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

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <osgi/framework/Bundle.h>
#include <nosgi/framework/BundleImpl.h>
#include <nosgi/framework/BundleListener.h>
#include <nosgi/framework/Filter.h>
#include <nosgi/framework/FrameworkListener.h>
#include <nosgi/framework/ServiceReference.h>
#include <nosgi/framework/ServiceListener.h>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include <string>

using namespace std;

/**
 * @brief The OSGi framework class.
 */
class Framework : public Bundle {
public:

	/**
	 * @brief Install a bundle to the framework.
	 * 
	 * @param bundle Pointer to the bundle object to be installed.
	 * 
	 * @return Pointer to the bundle object installed.
	 */
	static Bundle *installBundle(Bundle *bundle);

	/**
	 * @brief Returns bundle object based on an ID.
	 * 
	 * @param id Identifier of the bundle object.
	 * 
	 * @return Pointer to the bundle object for given ID or NULL if the object
	 *         for the ID was not found.
	 */
	static Bundle *getBundle(long id);

	/**
	 * @todo
	 * 
	 * @return
	 */
	static multimap<string,Bundle *> *getBundles();

	/**
	 * @brief Returns collection of bundle objects installed.
	 * 
	 * It is up to the caller of this method to free the collection returned.
	 * 
	 * @return Pointer to the collection of bundle objects installed.
	 * 
	 * @see Framework::installBundle
	 */
	static vector<Bundle *> *getInstalledBundles();

	/**
	 * @brief Uninstalls bundle based on its ID.
	 * 
	 * @param id Identifier of the bundle object to be uninstalled.
	 */
	static void uninstallBundle(long id);

	/**
	 * @brief Registers listener to framework events.
	 * 
	 * @param listener Pointer to an object implementing the FrameworkListener
	 *                 interface to be notified on a framework event.
	 * 
	 * @see FrameworkEvent
	 */
	static void addFrameworkListener(FrameworkListener *listener);

	/**
	 * @brief Notifies registered listeners on a framework event.
	 * 
	 * @param event Reference to the event that occurred in the framework.
	 */
	static void notifyFrameworkListeners(FrameworkEvent &event);

	/**
	 * @brief Unregisters listener to framework events.
	 * 
	 * @param listener Pointer to an object implementing the FrameworkListener
	 *                 interface to be unregistered.
	 */
	static void removeFrameworkListener(FrameworkListener *listener);

	/**
	 * @brief Registers listener to bundle events.
	 * 
	 * @param listener Pointer to an object implementing the BundleListener
	 *                 interface to be notified a bundle event.
	 * 
	 * @see BundleEvent
	 */
	static void addBundleListener(BundleListener *listener);

	/**
	 * @brief Notifies registered listeners on a bundle event.
	 * 
	 * @param event Reference to the event that occurred in a bundle.
	 */
	static void notifyBundleListeners(BundleEvent &event);

	/**
	 * @brief Unregisters listener to bundle events.
	 * 
	 * @param listener Pointer to an object implementing the BundleListener
	 *                 interface to be unregistered.
	 */
	static void removeBundleListener(BundleListener *listener);

	/**
	 * @brief Registers listener to service events.
	 * 
	 * @param listener Pointer to an object implementing the ServiceListener
	 *                 interface to be notified a service event.
	 * 
	 * @see ServiceEvent
	 */
	static void addServiceListener(ServiceListener *listener, Filter *filter);

	/**
	 * @brief Notifies registered listeners on a service event.
	 * 
	 * @param event Reference to the event that occurred in a service.
	 */
	static void notifyServiceListeners(ServiceEvent &event);

	/**
	 * @brief Unregisters listener to service events.
	 * 
	 * @param listener Pointer to an object implementing the ServiceListener
	 *                 interface to be unregistered.
	 */
	static void removeServiceListener(ServiceListener *listener);

	/**
	 * @todo
	 * 
	 * @param id
	 * 
	 * @param service
	 */
	static void addServiceRegistration(long id, void *service);

	/**
	 * @brief Return service object based for given service reference.
	 * 
	 * @param ref Pointer to the service reference object
	 * 
	 * @return Pointer to the service object corresponding to the service
	 *         reference on success or NULL if the service reference is
	 *         invalid.
	 */
	static void *getService(const ServiceReference *ref);

	/**
	 * @todo
	 * 
	 * @param id
	 */
	static void removeServiceRegistration(long id);

	/**
	 * @brief Prints list of services to the standard output.
	 */
	static void printServices();

	/**
	 * @todo
	 * 
	 * @param ref
	 * @param interface
	 */
	static void addInterfaceRegistration(ServiceReference *ref,
			const string &interface);

	/**
	 * @brief Returns service reference object for the given service name.
	 * 
	 * @param interfaceName String representing the name under which the service
	 *                      is registered in the framework. 
	 * 
	 * @return Pointer to the service reference object for the given name
	 *         on success or NULL if the service name is unknown to the framework.
	 */
	static ServiceReference *getServiceReference(
			const string &interfaceName);

	/**
	 * @todo
	 * 
	 * @param id
	 * @param interface
	 */
	static void removeInterfaceRegistration(long id,
			const string &interface);

	/**
	 * @brief Generates a new, unique bundle ID.
	 * 
	 * @return New bundle ID.
	 */
	static long getNextBundleId();

	/**
	 * @brief Generates a new, unique service ID.
	 * 
	 * @return New service Id.
	 */
	static long getNextServiceId();

	/**
	 * @brief Loads bundles that were loaded in the framework at the time of its last shutdown.
	 */
	static void preload();

	/**
	 * @brief Framework initialization method.
	 */
	static void startup();

	/**
	 * @brief Performs framework cleanup on its shutdown.
	 * 
	 * The method may also save the framework state to be restored next time
	 * the framework is started.
	 * 
	 * @see Framework::preload
	 */
	static void shutdown();

	virtual long getBundleId() const;
	virtual string &getLocation();
	virtual string &getSymbolicName();
	virtual string getVersionString();
	virtual int getState() const;
	virtual void start() throw(BundleException);
	virtual void stop();
	virtual void update() throw(BundleException);
	virtual void uninstall() throw(BundleException);
	virtual Package *isExport(string &package) const;

private:
	static set<FrameworkListener *> frameworkListeners;
	static set<BundleListener *> syncBundleListeners;
	static map<ServiceListener *, Filter *> serviceListeners;
	static map<long,void *> serviceRegistry;
	static multimap<string,ServiceReference *> interfaceRegistry;
	static long bid;
	static long fid;
};

#endif
