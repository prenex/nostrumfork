/**
 * @file FrameworkListener.h
 * 
 * @brief Declaration of the FrameworkListener interface.
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

#ifndef NOSGI_FRAMEWORK_FRAMEWORKLISTENER_H
#define NOSGI_FRAMEWORK_FRAMEWORKLISTENER_H

#include <nosgi/framework/FrameworkEvent.h>

/**
 * @brief Interface of a listener to the framework related events.
 */
class FrameworkListener {
public:
    
	/**
	 * @brief Method called on an event that occurred at the framework level.
	 * 
	 * @param event Reference to the object containing data related to the event.
	 */
	virtual void frameworkEvent(const FrameworkEvent &event) = 0;
};

#endif
