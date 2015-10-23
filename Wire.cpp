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

#include <nosgi/framework/Wire.h>

Wire::Wire(string &forPackage) {
	package = forPackage;
}

string &Wire::getPackageName() {
	return package;
}

int Wire::getBundleId() {
	return bundleId;
}

void Wire::setBundleId(int newBundleId) {
	bundleId = newBundleId;
}

Version &Wire::getVersion() {
	return version;
}

void Wire::setVersion(Version &v) {
	version = v;
}

void *Wire::getHandle() {
	return handle;
}

void Wire::setHandle(void *h) {
	handle = h;
}
