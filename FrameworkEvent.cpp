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

#include <nosgi/framework/FrameworkEvent.h>

FrameworkEvent::FrameworkEvent(int t, Bundle *b, exception *e) :
	type(t), bundle(b), except(e) {
}

Bundle *FrameworkEvent::getBundle() const {
	return bundle;
}

exception *FrameworkEvent::getException() const {
	return except;
}

int FrameworkEvent::getType() const {
	return type;
}
