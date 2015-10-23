/**
 * @file InvalidSyntaxException.h
 * 
 * @brief Declaration of the InvalidSyntaxException class.
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

#ifndef NOSGI_FRAMEWORK_INVALIDSYNTAXEXCEPTION_H
#define NOSGI_FRAMEWORK_INVALIDSYNTAXEXCEPTION_H

#include <exception>

/**
 * @brief Exception class used to indicate a syntax error in text processing.
 */
class InvalidSyntaxException : public std::exception {
public:

	/**
	 * @brief Constructor
	 * 
	 * @param m Pointer to the string describing the exception.
	 */
	InvalidSyntaxException(const char *m) : msg(m) {};

	/**
	 * @brief Returns description of the exception.
	 * 
	 * @return String representing description of the exception.
	 */
	const char *what() { return msg; };

private:
	const char *msg;
};

#endif
