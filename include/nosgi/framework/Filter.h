/**
 * @file Filter.h
 * 
 * @brief Declaration of the Filter interface.
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

#ifndef NOSGI_FRAMEWORK_FILTER_H
#define NOSGI_FRAMEWORK_FILTER_H

#include <map>
#include <string>
#include <nosgi/framework/Property.h>
#include <nosgi/framework/ServiceReference.h>

/**
 * @brief Interface class for a RFC 1960-based filter.
 * 
 * The implementation classes provide additional methods to set up the filtering
 * criteria. This interface defines just the matching filter methods so that
 * other object can test its properties depending on the filter criteria.
 */
class Filter {
public:
	/*
	 * Destructor
	 */
	virtual ~Filter() {};

	/**
	 * @brief Filter using a service's properties.
	 * 
	 * @param ref Pointer to the service reference whose properties are used
	 *            in the match. 
	 * 
	 * @return True if the service's properties match this filter; false otherwise.
	 */
	virtual bool match(const ServiceReference &ref) = 0;

	/**
	 * @brief Filter using a dictionary with key/value pairs.
	 * 
	 * @param r Pointer to a map (dictionary) whose key/value pairs are used
	 *          in the match. 
	 * 
	 * @return True if the dictionary's values match this filter; false otherwise. 
	 */
	virtual bool match(const map<string,Property*> *r) = 0;

	//virtual string toString() = 0;
};

#endif
