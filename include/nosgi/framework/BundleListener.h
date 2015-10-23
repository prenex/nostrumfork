/**
 * @file BundleListener.h
 * 
 * @brief Declaration of the BundleListener interface.
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

#ifndef NOSGI_FRAMEWORK_BUNDLELISTENER_H
#define NOSGI_FRAMEWORK_BUNDLELISTENER_H

#include <nosgi/framework/BundleEvent.h>

class BundleEvent;

/**
 * @brief Interface of a listener to the bundle related events.
 */
class BundleListener {
public:
    
	/**
	 * @brief Method called on an event that occurred at the bundle level.
	 * 
	 * @param event Reference to the object containing data related to the event.
	 * 
	 */
	virtual void bundleChanged(const BundleEvent &event) = 0;
};

#endif
