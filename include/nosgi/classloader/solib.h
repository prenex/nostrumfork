/**
 * @file solib.h
 * 
 * @brief Declaration of interface for manipulation with symbols in shared libraries.
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

#ifndef NOSGI_FRAMEWORK_SOLIB_H
#define NOSGI_FRAMEWORK_SOLIB_H

#include <map>
#include <string>

using namespace std;

/**
 * @brief takes a list of symboles that should be readjusted to provide the
 *        visibity specified in the manifest file
 * 
 * @param filename shared object file
 * @param symbols list of symbols
 * 
 * @return error code
 */
extern int replaceSymbols(const char *filename, map<string,string> &symbols);

#endif
