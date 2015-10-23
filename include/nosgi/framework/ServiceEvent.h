/**
 * @file ServiceEvent.h
 * 
 * @brief Declaration of the ServiceEvent class.
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

#ifndef NOSGI_FRAMEWORK_SERVICEEVENT_H
#define NOSGI_FRAMEWORK_SERVICEEVENT_H

#include <nosgi/framework/ServiceReference.h>

/**
 * @brief Object containing information related to an service object event.
 */
class ServiceEvent {
public:
	
	/**
	 * @brief Set of event types that may occurr on a service. According to
	 * Spec R4 6.1.19
	 */
	enum serviceEvent {
		REGISTERED = 0x0001,
		MODIFIED = 0x0002,
		UNREGISTERING = 0x0004
	};
	
	/**
	 * @brief Constructor
	 * 
	 * @param type   Type of the service event.
	 * @param service Reference to the service object on which the event occurred.
	 */
	ServiceEvent(int type, ServiceReference &reference);

	/**
	 * @brief Returns service on which the event occurred.
	 * 
	 * @return Reference the service object on which the event occurred.
	 */
	ServiceReference &getServiceReference() const;

	/**
	 * @brief Returns type of the event.
	 * 
	 * @return Identifier of the event type as defined by the serviceEvent enum.
	 */
	int getType() const;

private:
	int type;
	ServiceReference &reference;
};

#endif
