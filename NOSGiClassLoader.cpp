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

#include <nosgi/classloader/NOSGiClassLoader.h>

#include <iostream>
#include <dlfcn.h>

using namespace std;

NOSGiClassLoader::NOSGiClassLoader() {
}

NOSGiClassLoader::~NOSGiClassLoader() {
}

void NOSGiClassLoader::setClassPath(const string &path) {
	classPath = path;
}

void* NOSGiClassLoader::loadActivator(const string &className) {
	if (loaded.count(className)>0) {
		used[className]++;
		return loaded[className];
	}
	void *handle;
	if (!(handle=dlopen((classPath+"/"+className+".so").c_str(),
			RTLD_LOCAL|RTLD_LAZY))) {
		cerr << dlerror() << endl;
		return NULL;
	}
	typedef void *activator_t();
	activator_t *newInstance = (activator_t *)dlsym(handle, "newInstance");
	if (!newInstance) {
		cerr << "no nosgi factory method found" << endl;
		dlclose(handle);
		return NULL;
	}
	used[className] = 1;
	loaded[className] = handle;
	return newInstance();
}

void NOSGiClassLoader::releaseActivator(const string &className) {
	if ((--used[className])==0) {
		dlclose(loaded[className]);
		used.erase(className);
		loaded.erase(className);
	}
}

void NOSGiClassLoader::loadSo(const string &soName) {
	void *handle;
	if(!(handle=dlopen((classPath+"/"+soName).c_str(),
				RTLD_LAZY|RTLD_LAZY))) {
		cerr << "could not load " << soName << endl;
		return;
	}
	loadedso[soName] = handle;
}

void NOSGiClassLoader::releaseSo(const string &soName) {
	void *handle;
	if((handle=loadedso[soName])) {
		dlclose(handle);
	}
	loadedso.erase(soName);
}

void NOSGiClassLoader::releaseAllSo() {
	for(map<string,void*>::iterator it=loadedso.begin(); it!=loadedso.end();
			it++) {
		if(dlclose(it->second)) {
			cerr << "could not unload" << it->first << endl;
		}
	}
	loadedso.clear();
}
