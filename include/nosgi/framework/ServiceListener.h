/**
 * @file ServiceListener.h
 * 
 * @brief Declaration of the ServiceListener interface.
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

#ifndef SERVICELISTERNER_H
#define SERVICELISTERNER_H

#include <nosgi/framework/ServiceEvent.h>

/**
 * @brief Interface of a listener to the service related events.
 */
class ServiceListener {
public:

	/**
	 * @brief Method called on an event that occurred at the service level.
	 * 
	 * @param event Reference to the object containing data related to the event.
	 */
	virtual void serviceChanged(ServiceEvent &event) = 0;
};

#endif
