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

#include <nosgi/service/NeededPackage.h>

NeededPackage::NeededPackage(const string &newName,
		VersionRange *newVersionRange)
		: name(newName), versionRange(newVersionRange), package(0) {
}

NeededPackage::~NeededPackage() {
	delete versionRange;
}

string NeededPackage::getName() const {
	return name;
}

VersionRange *NeededPackage::getVersionRange() {
	return versionRange;
}

void NeededPackage::setBundleId(long newBundleId) {
	bundleId = newBundleId;
}

long NeededPackage::getBundleId() const {
	return bundleId;
}

void NeededPackage::setPackage(Package *newPackage) {
	package = newPackage;
}

Package *NeededPackage::getPackage() {
	return package;
}
