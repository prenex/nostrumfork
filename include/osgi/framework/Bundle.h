/**
 * @file Bundle.h
 * 
 * @brief Declaration of the Bundle interface.
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

#ifndef BUNDLE_H
#define BUNDLE_H

#include <string>
#include <osgi/framework/BundleException.h>
#include <osgi/service/Package.h>

using namespace std;

/**
 * @brief OSGi bundle interface class.
 * 
 * A Bundle object is the access point to control the lifecycle of an installed bundle.
 * Each bundle installed in the OSGi environment must have an associated Bundle object. 
 */
class Bundle {
public:
	
	/**
	 * Set of possible states of the bundle.
	 */
	enum bundleState { //see Spec 6.1.4
		UNINSTALLED = 1,
		START_TRANSIENT = 1,
		STOP_TRANSIENT = 1,
		INSTALLED = 2,
		RESOLVED = 4,
		STARTING = 8,
		STOPPING = 16,
		ACTIVE = 32
	};

	virtual ~Bundle() {};

	/**
	 * @brief Returns the bundle's unique identifier.
	 * 
	 * @return Bundle unique identifier.
	 */
	virtual long getBundleId() const = 0;

	/**
	 * @brief Returns this bundle's location identifier.
	 * 
	 * The location identifier is the location passed to bundle context when
	 * a bundle is installed.
	 * 
	 * @return The string representation of this bundle's location identifier.
	 */
	virtual string &getLocation() = 0;

	/**
	 * @brief Returns the symbolic name of this bundle as specified in the manifest header.
	 * 
	 * @return The string representing this bundle's symbolic name or an empty
	 *         string if the bundle does not have a symbolic name.
	 */
	virtual string &getSymbolicName() = 0;

	/**
	 * @brief Returns the version of this bundle as specified by the manifest header.
	 * 
	 * @return The string representing this bundle's version.
	 */
	virtual string getVersionString() = 0;

	/**
	 * @brief Returns the state of this bundle 
	 * 
	 * @return State of this bundle.
	 *
	 * @see bundleState.
	 */
	virtual int getState() const = 0;

	/**
	 * @brief Starts this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is not in the RESOLVED state.
	 *        @see bundleState.
	 */
	virtual void start() throw(BundleException) = 0;

	/**
	 * @brief Stops this bundle.
	 */
	virtual void stop() = 0;

	/**
	 * @brief Uninstalls this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is being used by another bundle.
	 */
	virtual void uninstall() throw(BundleException) = 0;

	/**
	 * @brief Updates this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is not in the UNINSTALLED state.
	 *        @see bundleState.
	 */
	virtual void update() throw(BundleException) = 0;

	virtual Package *isExport(string &package) const = 0;
};

#endif
