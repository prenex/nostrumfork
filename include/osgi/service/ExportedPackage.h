/**
 * @file ExportedPackage.h
 * 
 * @brief Declaration of the ExportedPackage class.
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

#ifndef NOSGI_FRAMEWORK_EXPORTEDPACKAGE_H
#define NOSGI_FRAMEWORK_EXPORTEDPACKAGE_H

#include <nosgi/framework/Version.h>
#include <string>
#include <vector>

using namespace std;

class Bundle;

/**
 * @todo
 */
class ExportedPackage {
public:

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
	vector<Bundle*> getImportingBundles();   

	/**
	 * Returns version of the package.
	 * 
	 * @return Object representing the package version.
	 */
	Version getVersion() const;

	/**
	 * Checks if the package is going to be removed.
	 * 
	 * @return True if the package is pending for its removal; false otherwise.
	 */
	bool isRemovalPending() const;
};

#endif
