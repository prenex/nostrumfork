/**
 * @file BundleActivator.h
 * 
 * @brief Declaration of the BundleActivator interface.
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

#ifndef OSGI_FRAMEWORK_BUNDLEACTIVATOR_H
#define OSGI_FRAMEWORK_BUNDLEACTIVATOR_H

#define OSGI_SETACTIVATOR(x) \
	extern "C" BundleActivator* newInstance() { \
		return new x(); \
	}

#include <osgi/framework/BundleContext.h>

/**
 * @brief OSGi bundle activator interface class.
 *
 * The interface allows the OSGi framework to perform bundle-specific activities
 * on bundle start and stop.
 */
class BundleActivator { //see 4.3.6
	public:

		/**
		* Destructor
		*/
		virtual ~BundleActivator() {};

		/**
		 * @brief Method called when the bundle is started.
		 * 
		 * @param context The execution context of the bundle being started. 
		 */
		virtual void start(BundleContext &context) = 0;

		/**
		 * @brief Method called when the bundle is stopped.
		 * 
		 * @param context The execution context of the bundle being stopped. 
		 */
		virtual void stop(BundleContext &context) = 0;
};

#endif
