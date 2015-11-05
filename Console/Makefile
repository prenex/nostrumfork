FRAMEWORKDIR=..
CXX=g++
CXXFLAGS=-Wall -I$(FRAMEWORKDIR)/include -I../Clock -fPIC -g #-O2

all: Activator.so

Activator.o: Activator.h Activator.cpp

Console.o: Console.cpp Console.h

Activator.so: Activator.o Console.o
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^


zip: Activator.so META-INF/MANIFEST.MF
	zip bundle.zip $^

.PHONY: all clean

clean:
	rm -f *.o *.so bundle.zip
