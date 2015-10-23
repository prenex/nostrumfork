/**
 * @file NeededPackage.h
 * 
 * @brief Declaration of the NeededPackage class.
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

#ifndef NOSGI_FRAMEWORK_NEEDEDPACKAGE_H
#define NOSGI_FRAMEWORK_NEEDEDPACKAGE_H

#include <nosgi/framework/VersionRange.h>
#include <nosgi/service/ExportedPackage.h>
#include <nosgi/service/Package.h>
#include <string>
#include <vector>

using namespace std;

class Package;

/**
 * @todo
 */
class RequiredBundle {
public:

	/**
	 * Constructor
	 * 
	 * @todo
	 * 
	 * @param name
	 * @param versionRange
	 */
	RequiredBundle(const string &name, VersionRange *versionRange);

	/**
	 * Destructor
	 */
	~RequiredBundle();

	/**
	 * Returns name of the package.
	 * 
	 * @return String representing name of the package.
	 */
	Bundle &getBundle() const;

	/**
	 * @todo
	 * 
	 * @return
	 */
	string getSymbolicName();
	
	/**
	 * @todo
	 * 
	 * @return
	 */
	vector<Bundle*> getRequiringBundles();

	/**
	 * @todo
	 * 
	 * @return
	 */
	Version *getVersion();

	/**
	 * Checks if the package is going to be removed.
	 * 
	 * @return True if the package is pending for its removal; false otherwise.
	 */
	bool isRemovalPending() const;

	/**
	 * @todo
	 * 
	 * @param newBundleId
	 * 
	 * @return
	 */
	void setBundleId(long newBundleId);

	/**
	 * @todo
	 * 
	 * @return
	 */
	long getBundleId() const;

	/**
	 * @todo
	 * 
	 * @param package
	 * 
	 * @return
	 */
	void setPackage(Package *package);

	/**
	 * @todo
	 * 
	 * @return
	 */
	Package *getPackage();

private:
	string name;
	VersionRange *versionRange;
	long bundleId;
	Package *package;
};

#endif
