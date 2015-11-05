CPPFLAGS=-I/usr/include -I./include -Wall -std=c++11 -g #-O2
BINNAME=nosgi
BINDST=/usr/bin
INCLUDEDST=/usr/include

all: $(BINNAME)

$(BINNAME): framework.o Bundle.o BundleContext.o BundleEvent.o FrameworkEvent.o NeededPackage.o Package.o ServiceReference.o ServiceRegistration.o Version.o VersionRange.o NOSGiClassLoader.o solib.o ServiceEvent.o RFC1960Filter.o Property.o FrameworkEvent.o
	$(CXX) $^ -rdynamic -ldl -lpthread -lelf -o $@

doc: 
	doxygen Doxyfile

install: $(BINNAME)
	cp $(BINNAME) $(BINDST)/$(BINNAME)
	rm -fr $(INCLUDEDST)/nosgi/
	rm -fr $(INCLUDEDST)/osgi/
	mkdir $(INCLUDEDST)/nosgi/
	mkdir $(INCLUDEDST)/osgi/
	mkdir $(INCLUDEDST)/nosgi/framework
	cp include/nosgi/framework/*.h $(INCLUDEDST)/nosgi/framework
	mkdir $(INCLUDEDST)/nosgi/service
	cp include/nosgi/service/*.h $(INCLUDEDST)/nosgi/service
	mkdir $(INCLUDEDST)/osgi/service
	cp include/osgi/service/*.h $(INCLUDEDST)/osgi/service
	mkdir $(INCLUDEDST)/osgi/framework
	cp include/osgi/framework/*.h $(INCLUDEDST)/osgi/framework

uninstall:
	rm $(BINDST)/$(BINNAME)
	rm -r $(INCLUDEST)/nosgi/
	rm -r $(INCLUDEST)/osgi/

.PHONY: doc clean install
clean:
	rm -f *.o framework
	rm -rf doc
