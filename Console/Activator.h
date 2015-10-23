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

#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include <nosgi/framework/BundleActivator.h>
#include <nosgi/framework/BundleContext.h>

class Activator : public BundleActivator {
	public:
		virtual ~Activator();
		virtual void start(BundleContext &context);
		virtual void stop(BundleContext &context);
};

#endif
