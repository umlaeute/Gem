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

qtExt:
	$(CXX) $(cxxflags) $(LDADD) -I/usr/include/qt gemwindow_qtExt.cpp -o gemwindow_qtExt.pd_linux



pd_linux: qtExt
