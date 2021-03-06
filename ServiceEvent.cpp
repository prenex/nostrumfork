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

#include <nosgi/framework/ServiceEvent.h>

ServiceEvent::ServiceEvent(int t, ServiceReference &r)
	: type(t), reference(r) {

}

ServiceReference &ServiceEvent::getServiceReference() const {
	return reference;
}

int ServiceEvent::getType() const {
	return type;
}
