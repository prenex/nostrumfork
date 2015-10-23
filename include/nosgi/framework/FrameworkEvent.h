/**
 * @file FrameworkEvent.h
 * 
 * @brief Declaration of the FrameworkEvent class.
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

#ifndef NOSGI_FRAMEWORK_FRAMEWORKEVENT_H
#define NOSGI_FRAMEWORK_FRAMEWORKEVENT_H

#include <exception>

using namespace std;

class Bundle;

/**
 * @brief Object containing information related to an framework event.
 */
class FrameworkEvent {
public:

	/**
	 * @brief Set of event types that may occurr in the framework. According
	 * to Spec R4 6.1.14
	 */
	enum frameworkEvent {
		STARTED = 0x0001,
		ERROR = 0x0002,
		PACKAGES_REFRESHED = 0x0004,
		STARTLEVEL_CHANGED = 0x0008,
		WARNING = 0x0010,
		INFO = 0x0020
	};

	/**
	 * @brief Constructor
	 *
	 * @param type   Type of the bundle event.
	 * @param bundle Pointer to the bundle object associated with the event.
	 * @param except Pointer to the exception object associated with the event.
	 */
	FrameworkEvent(int type, Bundle *bundle, exception *except);

	/**
	 * @brief Returns bundle associated with the event.
	 * 
	 * @return Pointer the bundle associated with the event.
	 */
	Bundle *getBundle() const;

	/**
	 * @brief Returns exception associated with the event.
	 * 
	 * @return Pointer the exception associated with the event.
	 */
	exception *getException() const;

	/**
	 * @brief Returns type of the event.
	 * 
	 * @return Identifier of the event type as defined by the frameworkEvent enum.
	 */
	int getType() const;

private:
	int type;
	Bundle *bundle;
	exception *except;
};

#endif
