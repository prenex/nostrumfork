/**
 * @file Wire.h
 * 
 * @brief Declaration of the Wire class.
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

#ifndef NOSGI_FRAMEWORK_WIRE_H
#define NOSGI_FRAMEWORK_WIRE_H

#include <string>
#include <nosgi/framework/Version.h>

using namespace std;

/**
 * @todo
 */
class Wire {
public:

	/**
	 * @brief Constructor
	 * @todo
	 * 
	 * @param forPackage
	 */
	Wire(string &forPackage);

	/**
	 * @todo
	 * 
	 * @return
	 */
	string& getPackageName();

	/**
	 * @todo
	 * 
	 * @param bundleId
	 */
	void setBundleId(int bundleId);

	/**
	 * @todo
	 * 
	 * @return
	 */
	int getBundleId();

	/**
	 * @todo
	 * 
	 * @return
	 */
	Version& getVersion();

	/**
	 * @todo
	 *
	 * @param
	 */
	void setVersion(Version &version);

	/**
	 * @todo
	 * 
	 * @return
	 */
	void *getHandle();

	/**
	 * @todo
	 * 
	 * @param
	 */
	void setHandle(void *);

private:
	string package;
	int bundleId;
	Version version;
	void *handle;
};

#endif
