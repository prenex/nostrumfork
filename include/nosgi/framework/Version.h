/**
 * @file Version.h
 * 
 * @brief Declaration of the Version class.
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

#ifndef NOSGI_FRAMEWORK_VERSION_H
#define NOSGI_FRAMEWORK_VERSION_H

#include <string>
#include <sstream>

using namespace std;

/**
 * @brief Version identifier class for bundles and packages.
 * 
 * The version identifier consists of:
 * @li major number
 * @li minor number
 * @li micro number
 * @li qualifier string
 */
class Version {
public:

	/**
	 * @brief Parses the version identifier from a stream of strings.
	 * 
	 * @param stream Stream containing comma separated version identifier
	 *               in the format "<major>.<minor>.<micro>.<qualifier>".
	 */
	void parseVersion(istringstream &stream);

	/**
	 * @brief Returns string representation of the version identifier.
	 * 
	 * @return String in the format "<major>.<minor>.<micro>.<qualifier>".
	 */
	string getVersionString() const;

	/**
	 * @brief Tests this version to another one for equality.
	 * 
	 * A version is equal to another one if all its components i.e. the major
	 * number, minor number, micro number and the qualifier string are equal.
	 * 
	 * @param version The version identifier to compare to.
	 * 
	 * @return True if the version identifiers are equal; false otherwise.
	 */
	bool operator==(const Version &version) const;

	/**
	 * @brief Tests this version is smaller than another one.
	 * 
	 * A version identifier is smaller that another one if either
	 * @li its major number is smaller
	 * @li the major numbers are equal, and its minor number is smaller
	 * @li the major, and minor numbers are equal, and its micro number is smaller
	 * @li the major, minor, and micro numbers are equal, and its qualifier string is smaller
	 * 
	 * @param version The version identifier to compare to.
	 * 
	 * @return True if this version identifier is smaller; false otherwise.
	 */
	bool operator<(const Version &version) const;

	/**
	 * @brief Tests this version is greater than another one.
	 * 
	 * A version identifier is greater that another one if either
	 * @li its major number is greater
	 * @li the major numbers are equal, and its minor number is greater
	 * @li the major, and minor numbers are equal, and its micro number is greater
	 * @li the major, minor, and micro numbers are equal, and its qualifier string is greater
	 * 
	 * @param version The version identifier to compare to.
	 * 
	 * @return True if this version identifier is greater; false otherwise.
	 */
	bool operator>(const Version &version) const;

	/**
	 * @brief Major number of the version identifier.
	 */
	unsigned short major;

	/**
	 * @brief Minor number of the version identifier.
	 */
	unsigned short minor;

	/**
	 * @brief Micro number of the version identifier.
	 */
	unsigned short micro;

	/**
	 * @brief Qualifier string part of the version identifier.
	 */
	string qualifier; 
};

#endif
