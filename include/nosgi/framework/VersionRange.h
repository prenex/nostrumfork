/**
 * @file VersionRange.h
 * 
 * @brief Declaration of the VersionRange class.
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

#ifndef NOSGI_FRAMEWORK_VERSIONRANGE_H
#define NOSGI_FRAMEWORK_VERSIONRANGE_H

#include <nosgi/framework/Version.h>
#include <sstream>

/**
 * @brief Class specifying range of versions for bundles and packages.
 * 
 * @see Version
 */
class VersionRange {
public:

	/**
	 * @brief Constructor
	 * 
	 * @param minVersion Reference to the Version object representing the low end
	 *                   of the range.
	 * @param includeMin Flag indicating if the low end version is included
	 *                   in the range.
	 * @param maxVersion Reference to the Version object representing the high end
	 *                   of the range.
	 * @param includeMax Flag indicating if the high end version is included
	 *                   in the range.
	 */
	VersionRange(const Version &minVersion, bool includeMin,
			const Version &maxVersion, bool includeMax);

	/**
	 * @brief Constructor
	 * 
	 * @param stream Reference to the stream of strings from which the VersionRange
	 *               can be initialized.
	 * 
	 * The format is two comma separated patterns of Version object string representation
	 * enclosed in either ( ) (ends excluded) or [ ] (ends included) parentheses.
	 * @par 
	 * Example: "(<major>.<minor>.<micro>.<qualifier>, <major>.<minor>.<micro>.<qualifier>]"
	 * 
	 * @see Version::parseVersion for the Version object string pattern.
	 */
	VersionRange(istringstream &stream);

	/**
	 * @brief Tests if the a version range represents a subset of this range object.
	 * 
	 * @param version Reference to the VersionRange object to be compared with
	 *                this one.
	 * 
	 * @return True if the parameter is a subset of this range; false otherwise,
	 */
	bool isIncluded(const Version &version);

private:
	Version minVersion;
	bool includeMin;
	Version maxVersion;
	bool includeMax;
};

#endif
