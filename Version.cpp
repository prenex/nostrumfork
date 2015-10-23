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

#include <nosgi/framework/Version.h>
#include <sstream>
#include <iostream>

void Version::parseVersion(istringstream &stream) {
	int points = 0;
	char point = '.';
	while (point == '.' && points < 4) {
		switch (points) {
		case 0: stream >> major;
			break;
		case 1: stream >> minor;
			break;
		case 2: stream >> micro;
			break;
		case 3:	stream >> qualifier;
			break;
		}
		points++;
		stream >> point;
	}
	stream.unget();
}

bool Version::operator==(const Version &version) const {
	return (version.major == major && version.minor == minor
		&& version.micro == micro);
}

bool Version::operator<(const Version &version) const {
	if (major < version.major) return true;
	if (major == version.major) {
		if (minor < version.minor) return true;
		if (minor == version.minor) {
			return micro < version.micro;
		}
	}
	return false;
}

bool Version::operator>(const Version &version) const {
	if (major > version.major) return true;
	if (major == version.major) {
		if (minor > version.minor) return true;
		if (minor == version.minor) {
			return micro > version.micro;
		}
	}
	return false;
}

string Version::getVersionString() const {
	stringstream versionString;
	versionString << (major>9?"":"0") << major << '.'
		 << (minor>9?"":"0") << minor << '.'
		 << (micro>9?"":"0") << micro;
	return versionString.str();
}
