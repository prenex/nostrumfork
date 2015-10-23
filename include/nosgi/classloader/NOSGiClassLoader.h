/**
 * @file NOSGiClassLoader.h
 * 
 * @brief Declaration of the NOSGiClassLoader class.
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

#ifndef NOSGI_CLASSLOADER_NOSGICLASSLOADER_H
#define NOSGI_CLASSLOADER_NOSGICLASSLOADER_H

#include <string>
#include <map>

/**
 * @brief Handles manipulation with bundle libraries.
 */
class NOSGiClassLoader {
public:

	/**
	 * Constructor
	 */
	NOSGiClassLoader();

	/**
	 * Destructor
	 */
	virtual ~NOSGiClassLoader();

	/**
	* @brief Sets path where the loader searches for libraries to load.
	* 
	* @param pathlist Reference to the string representing the path where
	*                 the libraries should be searched.
	*/
	virtual void setClassPath(const std::string &pathlist);

	/**
	 * @brief Loads library that contains implementation of an activator.
     * 
	 * If the library is already loaded, the existing instance of the activator
	 * is returned and its reference counter is increased.
	 * 
	 * @param className Reference to a string representing the name of the library
	 *                  to load without the .so suffix.  
	 * 
	 * @return Pointer to the object implementing the BundleActivator interface
	 *         on success or NULL on error.
	 * 
	 * @see setClassPath
	 */
	virtual void *loadActivator(const std::string &className);

	/**
	 * @brief Unloads library that contains implementation of an activator.
	 * 
	 * The library is really unloaded only if it is not used anymore. In other
	 * words, the number of releaseActivator calls must correspond to the number
	 * of previous NOSGiClassLoader::loadActivator calls.
	 * 
	 * @param Reference to a string representing the name of the library
	 *                  to be loaded (without the .so suffix).
	 */
	virtual void releaseActivator(const std::string &className);

	/**
	 * @brief Loads a shared library to the process space.
	 * 
	 * @param soName Reference to a string representing the name of the library
	 *               (including the .so suffix) to load .
	 * 
	 * @see setClassPath
	 */
	virtual void loadSo(const std::string &soName);

	/**
	 * @brief Unloads a shared library from the process space.
	 * 
	 * Only libraries previously loaded using the NOSGiClassLoader::loadSo method
	 * can be unloaded this way.
	 * 
	 * @param soName Reference to a string representing the name of the library
	 *               (including the .so suffix) to be unloaded .
	 */
	virtual void releaseSo(const std::string &soName);

	/**
	 * @brief Unloads all shared libraries previously loaded from the process space.
	 * 
	 * Only libraries previously loaded using the NOSGiClassLoader::loadSo method
	 * can be unloaded this way.
	 */
	virtual void releaseAllSo();

private:
	std::string classPath;
	std::map<std::string, void *> loaded;
	std::map<std::string, unsigned int> used;
	std::map<std::string, void *> loadedso;
};

#endif
