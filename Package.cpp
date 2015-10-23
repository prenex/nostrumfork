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

#include <osgi/service/Package.h>

Package::Package(const string &newName)
	: name(newName), imports(0) {

}

Package::Package(const string &newName, const Version &newVersion)
	: name(newName), version(newVersion), imports(0) {

}

string Package::getName() const {
	return name;
}

Version &Package::getVersion() {
	return version;
}

void Package::setBundleId(long newBundleId) {
	bundleId = newBundleId;
}

long Package::getBundleId() const {
	return bundleId;
}

void Package::incImports() {
	imports++;
}

void Package::decImports() {
	imports--;
}

bool Package::isImported() const {
	return imports != 0;
}
