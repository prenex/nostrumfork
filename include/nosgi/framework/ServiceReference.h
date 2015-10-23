/**
 * @file ServiceReference.h
 * 
 * @brief Declaration of the ServiceReference class.
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

#ifndef NOSGI_FRAMEWORK_SERVICEREFERENCE_H
#define NOSGI_FRAMEWORK_SERVICEREFERENCE_H

#include <map>
#include <string>
#include <nosgi/framework/Property.h>
#include <nosgi/framework/ServiceRegistration.h>

using namespace std;

/**
 * @brief A reference to a service.
 * 
 * The service reference objects allow one service object to be shared between
 * bundles. The service reference provides access to the service and its properties.
 */
class ServiceReference {
public:

	/**
	 * @brief Constructor
	 * 
	 * @param reg Pointer to the service registration object. This object is
     *            unique to every service registered in the framework and may
     *            be shared by multiple service references.
	 */
	ServiceReference(ServiceRegistration* reg);

	/**
	 * @todo modify this to getBundle()
	 * 
	 * @return
	 */
	long getId() const;

	/**
	 * @brief Set of properties associated with the service.
	 * 
	 * A property is a pair - property name and its value.
	 * @see Property
	 */
	map<string,Property*> *properties;

private:
	ServiceRegistration *serviceRegistration;
};

#endif
