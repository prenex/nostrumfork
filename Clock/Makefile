FRAMEWORKDIR=..
CXX=g++
CXXFLAGS=-fPIC -Wall -I$(FRAMEWORKDIR)/include -O2

all: Activator.so

Activator.o: Activator.cpp Clock.h

Clock.o: Clock.cpp Clock.h

org.osgi.time-_nosgi-exported_.so: Clock.o
	$(CXX) -shared -Wl,-soname,$@ -Wl,-rpath,_nosgi-bundle-working-dir_ -o $@ $^

Activator.so: Activator.o org.osgi.time-_nosgi-exported_.so
	$(CXX) -shared -Wl,-soname,$@ -Wl,-rpath,_nosgi-bundle-working-dir_ -o $@ $^

zip: Activator.so org.osgi.time-_nosgi-exported_.so META-INF/MANIFEST.MF
	zip bundle.zip $^

.PHONY: all clean

clean:
	rm -f *.o *.classname *.so *.zip
