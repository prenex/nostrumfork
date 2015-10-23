/**
 * @file RFC1960Filter.h
 * 
 * @brief Declaration of the RFC1960Filter class.
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

#ifndef NOSGI_FRAMEWORK_RFCFILTER1960_H
#define NOSGI_FRAMEWORK_RFCFILTER1960_H

//import org.osgi.framework.InvalidSyntaxException;
#include <algorithm>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>
#include <nosgi/framework/Filter.h>
#include <nosgi/framework/Property.h>
#include <nosgi/framework/ServiceReference.h>

using namespace std;

/**
 * @brief Implementation class of the RFC 1960-based filter.
 */
class RFC1960Filter : public Filter {
private:
	enum operators {
		AND_OPERATOR = 1,
		OR_OPERATOR,
		NOT_OPERATOR
	};
	enum comp {
		EQUALS = 0,
		PRESENT,
		APPROX,
		GREATER,
		LESS
	};

	//const string[] OP = { "=", "=*", "~=", ">=", "<=" };
	//const class[] STRINGCLASS = new class[] { String.class };

	/*static final Filter NULL_FILTER = new Filter() {
		public boolean match(final ServiceReference reference) {
			return true;
		}

		public boolean match(final Dictionary dictionary) {
			return true;
		}
	};*/

	vector<Filter*> operands;
	int operat;

	RFC1960Filter(int op);

public:

	/**
	 * @brief Filter factory method.
	 * 
	 * @todo
	 * 
	 * @param filterString String representation of the filtering criteria.
	 */
	static Filter *fromString(const string &filterString)
			/*throws InvalidSyntaxException*/;

	/**
	 * @brief Filter using a service's properties.
	 * 
	 * @param ref Pointer to the service reference whose properties are used
	 *            in the match. 
	 * 
	 * @return True if the service's properties match this filter; false otherwise.
	 */
	bool match(const ServiceReference &reference);

	/**
	 * @brief Filter using a dictionary with key/value pairs.
	 * 
	 * @param r Pointer to a map (dictionary) whose key/value pairs are used
	 *          in the match. 
	 * 
	 * @return True if the dictionary's values match this filter; false otherwise. 
	 */
	bool match(const map<string,Property*> *values);

	string toString();
	//bool equals(final Object obj);
	int hashCode();

	/**
	* @brief Simple RFC 1960-based filter.
	* 
	* @todo
	*/
	class RFC1960SimpleFilter : public Filter {
	private:
		const string id;
		const int comparator;
		const string value;

	public:

		/**
		 * @brief Constructor.
		 * 
		 * @todo
		 * 
		 * @param i
		 * @param comp
		 * @param v
		 */
		RFC1960SimpleFilter(const string &i, const int comp,
				const string &v);

		/**
		* @brief Filter using a dictionary with key/value pairs.
		* 
		* @param r Pointer to a map (dictionary) whose key/value pairs are used
		*          in the match. 
		* 
		* @return True if the dictionary's values match this filter; false otherwise. 
		*/
		bool match(const map<string,Property*> *m);

		/**
		* @brief Filter using a dictionary with key/value pairs.
		* 
		* @param r Pointer to a map (dictionary) whose key/value pairs are used
		*          in the match. 
		* 
		* @return True if the dictionary's values match this filter; false otherwise. 
		*/
		bool match(const ServiceReference &reference);

	private:
		static bool compareString(const string &val,
				const int comparator, const string *attr);
/*		static boolean compareNumber(final String value,
				final int comparator, final Number attr);

		static boolean compareTyped(final Object typedVal,
				final int comparator, final Comparable attr);
		static boolean compareArray(final String value,
				final int comparator, final Object[] array);
		static boolean compareReflective(final String val,
				final int comparator, final Comparable attr);
		static String stripWhitespaces(final String s);*/
		static int stringCompare(const string &c1, int p1,
				const string &c2, int p2);
/*	public:
       		String toString();
		bool equals(final Object obj);
		int hashCode();*/
	};

};

#endif
