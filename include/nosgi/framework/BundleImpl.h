/**
 * @file BundleImpl.h
 * 
 * @brief Declaration of the BundleImpl class.
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

#ifndef BUNDLEIMPL_H
#define BUNDLEIMPL_H

#include <string>
#include <vector>
#include <osgi/framework/Bundle.h>
#include <osgi/framework/BundleActivator.h>
#include <osgi/framework/BundleException.h>
#include <nosgi/framework/BundleContextImpl.h>
#include <nosgi/classloader/NOSGiClassLoader.h>
#include <nosgi/framework/Version.h>
#include <osgi/service/Package.h>
#include <nosgi/service/NeededPackage.h>

using namespace std;

/**
 * @brief Implementation class of the Bundle interface.
 */
class BundleImpl : public Bundle {
public:

	/**
	 * @brief Constructor in order to install a new bundle
	 * 
	 * @param location location to load bundle from
	 */
	BundleImpl(const string& location) throw(BundleException);

	/**
	 * @brief Constructor in order to load a new bundle from bundle cache
	 * 
	 * @param bundleId bundle id to load from cache
	 */
	BundleImpl(long bundleId);

	/**
	 * @brief Destructor
	 */
	virtual ~BundleImpl();

	/**
	 * @brief Returns the bundle's unique identifier.
	 * 
	 * @return Bundle unique identifier.
	 */
	virtual long getBundleId() const;

	/**
	 * @brief Returns this bundle's location identifier.
	 * 
	 * The location identifier is the location passed to bundle context when
	 * a bundle is installed.
	 * 
	 * @return The string representation of this bundle's location identifier.
	 */
	virtual string &getLocation();

	/**
	 * @brief Returns the symbolic name of this bundle as specified in the manifest header.
	 * 
	 * @return The string representing this bundle's symbolic name or an empty
	 *         string if the bundle does not have a symbolic name.
	 */
	virtual string &getSymbolicName();

	/**
	 * @brief Returns the version of this bundle as specified by the manifest header.
	 * 
	 * @return The string representing this bundle's version.
	 */
	virtual string getVersionString();

	/**
	 * @brief Returns the state of this bundle 
	 * 
	 * @return State of this bundle.
	 *
	 * @see bundleState.
	 */
	virtual int getState() const;

	/**
	 * @brief Starts this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is not in the RESOLVED state.
	 *        @see bundleState.
	 */
	virtual void start() throw(BundleException);

	/**
	 * @brief Stops this bundle.
	 */
	void stop();

	/**
	 * @brief Uninstalls this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is being used by another bundle.
	 */
	virtual void uninstall() throw(BundleException);

	/**
	 * @brief Updates this bundle.
	 * 
	 * @throw BundleException thrown if the bundle is not in the UNINSTALLED state.
	 *        @see bundleState.
	 */
	virtual void update() throw(BundleException);

	virtual Package *isExport(string &package) const;
	void doUpdate();

private:
	bool parseManifest(const string &location);
	//string getDirName(const string &filename);
	void resolveAndLoad(bool setBundleId) throw(BundleException);
	void resolve();
	void addImports(const string &file, bool setBundleId) const;
	void exportPackages(bool setBundleId) const;
	void unExportPackages();
	Version *doResolve(Package *package, NeededPackage *neededPackage,
		                Version *version, long bid);
	void doUninstall();
	void saveState();
	bundleState state;
	Version version;
	long bundleId;
	string location;
	string cachelocation;
	string symbolicName;
	BundleActivator *activator;
	NOSGiClassLoader *classLoader;
	string activatorName;
	BundleContextImpl *context;
	vector<NeededPackage *> imports;
	vector<Package *> exports;
	bool sourcebundle;
};

#endif
