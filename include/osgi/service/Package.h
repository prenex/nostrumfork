/**
 * @file Package.h
 * 
 * @brief Declaration of the Package class.
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

#ifndef NOSGI_FRAMEWORK_PACKAGE_H
#define NOSGI_FRAMEWORK_PACKAGE_H

#include <nosgi/framework/Version.h>
#include <osgi/service/ExportedPackage.h>
#include <string>

using namespace std;

class Bundle;

/**
 * @todo
 */
class Package : ExportedPackage {
public:

	/**
	 * Constructor
	 * 
	 * @param name
	 */
	Package(const string &name);

	/**
	 * Constructor
	 * 
	 * @todo
	 * 
	 * @param name
	 * @param version
	 */
	Package(const string &name, const Version &version);

	/**
	 * Returns name of the package.
	 * 
	 * @return String representing name of the package.
	 */
	string getName() const;

	/**
	 * @todo
	 * 
	 * @return
	 */
	Bundle &getExportingBundle();

	/**
	 * @todo
	 * 
	 * @return
	 */
	Bundle *getImportingBundle();

	/**
	 * Returns version of the package.
	 * 
	 * @return Object representing the package version.
	 */
	Version &getVersion();

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
	 */
	void decImports();

	/**
	 * @todo
	 */
	void incImports();

	/**
	 * @todo
	 */
	bool isImported() const;

private:
	string name;
	Version version;
	long bundleId;
	int imports;
};

#endif
