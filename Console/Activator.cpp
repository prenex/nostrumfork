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

#include "Activator.h"
#include "Console.h"


OSGI_SETACTIVATOR(Activator)

Activator::~Activator() { }

void Activator::start(BundleContext &context) {
	Activator::console = new Console(context);
	Activator::console->run();
}

void Activator::stop(BundleContext &context) {
	Activator::console->stop();
	delete Activator::console;
}

