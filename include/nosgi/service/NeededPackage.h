/*
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
#include <osgi/framework/Bundle.h>
#include <osgi/service/ExportedPackage.h>
#include <osgi/service/Package.h>
#include <string>

using namespace std;

class Package;

class NeededPackage : ExportedPackage {
public:
	NeededPackage(const string &name, VersionRange *versionRange);
	~NeededPackage();
	string getName() const;
	Bundle &getExportingBundle();
	Bundle *getImportingBundle();
	VersionRange *getVersionRange();
	bool isRemovalPending() const;

	void setBundleId(long newBundleId);
	long getBundleId() const;
	void setPackage(Package *package);
	Package *getPackage();

private:
	string name;
	VersionRange *versionRange;
	long bundleId;
	Package *package;
};

#endif
