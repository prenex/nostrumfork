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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <osgi/framework/BundleContext.h>
#include <pthread.h>

class Console {
public:
	Console(BundleContext &context);
	void run();
	void stop();

private:
	bool running;
	BundleContext &context;
	static pthread_t tid;
	string getStateString(int state);
	static void *starter(void *instance);
	void loop();
};

#endif
