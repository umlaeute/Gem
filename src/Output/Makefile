current: pd_linux

cxxflags = -g -O2 -fPIC $(AFLAGS) -I../ -I/usr/local/include/
#cxxflags += -Wall -pedantic -ansi -Dunix -D__linux__
cxxflags += -fno-builtin -O3 -funroll-loops -ffast-math
cxxflags += $(CXXFLAGS)



CXXLINK = $(CXX) $(CXXFLAGS) -o $@
LDFLAGS =  -L/usr/X11R6/lib 
LDADD = -shared
LIBS = 
INCLUDES = 

#SYSTEMDOCS = /usr/local/share/doc/gem

extwin:
	$(CXX) $(cxxflags) $(LDADD) gemextwin.cpp -o gemextwin.pd_linux


pd_linux: extwin

clean:
	@rm -f *.pd_linux


distclean: clean
	@rm -f *~ *.o
