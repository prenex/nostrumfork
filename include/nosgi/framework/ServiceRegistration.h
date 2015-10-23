/**
 * @file ServiceRegistration.h
 * 
 * @brief Declaration of the ServiceRegistration class.
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

#ifndef NOSGI_FRAMEWORK_SERVICEREGISTRATION_H
#define NOSGI_FRAMEWORK_SERVICEREGISTRATION_H

#include <map>
#include <string>
#include <nosgi/framework/Property.h>

using namespace std;

class ServiceReference;

/**
 * @brief A registered service class.
 * 
 * The framework creates service registration object for each service that was
 * successfully registered in the framework.
 */
class ServiceRegistration {
public:

	/**
	 * @brief Constructor
	 * 
	 * @param interf String representing the name under which the service is
	 *               registered in the framework.
	 */
	ServiceRegistration(string& interf);

	/**
	 * @brief Destructor
	 */
	~ServiceRegistration();

	/**
	 * @brief Unregisters service from the framework.
	 * 
	 * No service reference objects can be used to access the service after
	 * completion of this call.
	 */
	void unregister();

	/**
	 * @todo
	 * 
	 * @return
	 */
	long getId() const;

	/**
	 * @todo
	 * 
	 * @return
	 */
	int getRanking() const;

	/**
	 * @brief Returns a service reference object for the registered service.
	 * 
	 * The service reference object may be shared between bundles.
	 * 
	 * @return Pointer to the service reference object.
	 */
	ServiceReference *getReference() const;

	/**
	 * @brief Sets (or updates) properties associated with a service.
	 * 
	 * @param props Pointer to the set of properties to be associated with
	 *              the service. A property is a pair - property name and
	 *              its value.
	 */
	void setProperties(map<string,Property*> *props);

private:
	string interface;
	ServiceReference *reference;
	long id;
	int ranking;
};

#endif
