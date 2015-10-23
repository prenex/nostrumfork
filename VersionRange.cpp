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

#include <nosgi/framework/VersionRange.h>
#include <iostream>

using namespace std;

VersionRange::VersionRange(const Version &minVersion, bool includeMin,
		const Version &maxVersion, bool includeMax) {

}

VersionRange::VersionRange(istringstream &stream) {
	//parse VersionRangs out of stringstream
	char c;
	stream >> c;
	if (c == '[' || c == '(') {
		includeMin = (c=='[');
		minVersion.parseVersion(stream);
		stream >> c;
		if (c!=',') {
			cerr << "parse error (" << c << ")" << endl;
		}
		maxVersion.parseVersion(stream);
		for (int i=0; i<6; i++) stream >> c;
		if (c != ']' && c != ')') {
			cerr << "parse error (" << c << ")" << endl;
		}
		includeMax = (c==']');
	} else {
		cerr << "parse error (" << c << ")" << endl;
	}
}

bool VersionRange::isIncluded(const Version &version) {
	if (version > minVersion && version < maxVersion) {
		return true;
	} else if (includeMin && version == minVersion) {
		return true;
	} else if (includeMax && version == maxVersion) {
		return true;
	} else {
		return false;
	}
}
