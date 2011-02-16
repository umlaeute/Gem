default: pd_objects

OBJDIR=./

include ../Makefile.subdir

all:
	make -C ..

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
GLUT=-framework glut
else
GLUT=-lglut
endif

%.$(EXT): %.o
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS) $<

gemglutwindow.$(EXT): gemglutwindow.o
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS)  $(GLUT) $<

gemcocoawindow.pd_darwin: gemcocoawindow.mm gemcocoawindow.h
	$(CXX) -o $@ -DGEM_INTERNAL $(GEM_EXTRA_CXXFLAGS) $(GEM_ARCH_CXXFLAGS) $(GEM_CXXFLAGS) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS) -framework Cocoa $<

## TESTING
## fetch the newest files for gemcocoawindow without having to resort to VCS
fetchcocoa:
	scp iem.at:~/src/cvs/pd-gem/Gem/src/Output/gemcocoawindow.mm .
	scp iem.at:~/src/cvs/pd-gem/Gem/src/Output/gemcocoawindow.h .

