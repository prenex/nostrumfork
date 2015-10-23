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

#include <nosgi/framework/Property.h>

Property::Property(string *s) {
	value.s = s;
	type = STRING;
}

Property::Property(int i) {
	value.i = i;
	type = INT;
}

Property::Property(short sh) {
	value.sh = sh;
	type = SHORT;
}

Property::Property(long l) {
	value.l = l;
	type = LONG;
}

Property::Property(float f) {
	value.f = f;
	type = FLOAT;
}

Property::Property(double d) {
	value.d = d;
	type = DOUBLE;
}

Property::~Property() {
	if (type==STRING) {
		delete value.s;
	}
}

short Property::getType() const {
	return type;
}

string *Property::getString() {
	return value.s;
}

int Property::getInt() const {
	return value.i;
}

long Property::getLong() const {
	return value.l;
}

short Property::getShort() const {
	return value.sh;
}

float Property::getFloat() const {
	return value.f;
}

double Property::getDouble() const {
	return value.d;
}
