/**
 * @file BundleException.h
 * 
 * @brief Declaration of the BundleException class.
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

#ifndef NOSGI_FRAMEWORK_BUNDLEEXCEPTION_H
#define NOSGI_FRAMEWORK_BUNDLEEXCEPTION_H

#include <exception>

/**
 * @brief Exception class used to indicate that a bundle lifecycle problem.
 */
class BundleException : public std::exception {
public:

	/**
	 * @brief Returns description of the exception.
	 * 
	 * @return String representing description of the exception.
	 */
	virtual const char *what() const throw() {
		return "BundleException";
	}
};

#endif
