/**
 * @file Property.h
 * 
 * @brief Declaration of the Property class.
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

#ifndef NOSGI_FRAMEWORK_PROPERTY_H
#define NOSGI_FRAMEWORK_PROPERTY_H

#include <string>

using namespace std;

/**
 * @brief Class representing property of different type.
 */
class Property {
public:

	/**
	 * @brief Set of supported property types.
	 */
	enum types {
		STRING = 0,
		INT,
		LONG,
		SHORT,
		FLOAT,
		DOUBLE
	};

	/**
	 * @brief Constructor of string type property.
	 * 
	 * @param s String property value.
	 */
	Property(string *s);

	/**
	 * @brief Constructor of integer type property.
	 * 
	 * @param i Integer property value.
	 */
	Property(int i);

	/**
	 * @brief Constructor of long integer type property.
	 * 
	 * @param l Long integer property value.
	 */
	Property(long l);

	/**
	 * @brief Constructor of short integer type property.
	 * 
	 * @param l Short integer property value.
	 */
	Property(short s);

	/**
	 * @brief Constructor of float type property.
	 * 
	 * @param f Float property value.
	 */
	Property(float f);

	/**
	 * @brief Constructor of double float type property.
	 * 
	 * @param d Double float property value.
	 */
	Property(double d);

	/**
	 * @brief Constructor of boolean type property.
	 * 
	 * @param b Boolean property value.
	 */
	Property(bool b);

	/**
	 * @brief Constructor of character type property.
	 * 
	 * @param c Character property value.
	 */
	Property(char c);

	/**
	 * @brief Destructor
	 */
	~Property();

	/**
	 * @brief Returns value of the string property.
	 * 
	 * @return String property value.
	 */
	string *getString();

	/**
	 * @brief Returns property type.
	 * 
	 * @return Property value.
	 * 
	 * @see types
	 */
	short getType() const;

	/**
	 * @brief Returns value of the integer property.
	 * 
	 * @return Integer property value.
	 */
	int getInt() const;

	/**
	 * @brief Returns value of the long integer property.
	 * 
	 * @return Long integer property value.
	 */
	long getLong() const;

	/**
	 * @brief Returns value of the short integer property.
	 * 
	 * @return Short integer property value.
	 */
	short getShort() const;

	/**
	 * @brief Returns value of the float property.
	 * 
	 * @return Float property value.
	 */
	float getFloat() const;

	/**
	 * @brief Returns value of the double float property.
	 * 
	 * @return Double float property value.
	 */
	double getDouble() const;

private:
	union {
		string *s;
		int i;
		long l;
		short sh;
		float f;
		double d;
		char c;
		bool b;
	} value;
	short type;
};

#endif
