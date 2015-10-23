/**
 * @file BundleEvent.h
 * 
 * @brief Declaration of the BundleEvent class.
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

#ifndef NOSGI_FRAMEWORK_BUNDLEEVENT_H
#define NOSGI_FRAMEWORK_BUNDLEEVENT_H

#include <osgi/framework/Bundle.h>

class Bundle;

/**
 * @brief Object containing information related to an event that occurred on a bundle.
 */
class BundleEvent {
public:

	/**
	 * @brief Set of event types that may occurr on a bundle. According to
	 * Spec R4 6.1.7
	 */
	enum bundleEvent {
		INSTALLED = 0x0001,
		STARTED = 0x0002,
		STOPPED = 0x0004,
		UPDATED = 0x0008,
		UNINSTALLED = 0x0010,
		RESOLVED = 0x0020,
		UNRESOLVED = 0x0040,
		STARTING = 0x0080,
		STOPPING = 0x0100,
		LAZY_ACTIVATION = 0x0200
	};

	/**
	 * @brief Constructor
	 * 
	 * @param type   Type of the bundle event.
	 * @param bundle Reference to the bundle object on which the event occurred.
	 */
	BundleEvent(int type, Bundle &bundle);
	
	/**
	 * @brief Returns bundle on which the event occurred.
	 * 
	 * @return Reference the bundle object on which the event occurred.
	 */
	Bundle &getBundle() const;

	/**
	 * @brief Returns type of the event.
	 * 
	 * @return Identifier of the event type as defined by the bundleEvent enum.
	 */
	int getType() const;

private:
	int type;
	Bundle &bundle;
};

#endif
