default: pd_objects

OBJDIR=./

include ../Makefile.subdir

all:
	make -C ..


GLUT=-lglut

%.$(EXT): %.o
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS) $<

gemglutwindow.$(EXT): gemglutwindow.o
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS)  $(GLUT) $<


gemcocoawindow.pd_darwin: gemcocoawindow.mm
	$(CXX) -o $@ -DGEM_INTERNAL $(GEM_EXTRA_CXXFLAGS) $(GEM_ARCH_CXXFLAGS) $(GEM_CXXFLAGS) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(GEM_LIBS) $(X_LIBS) $<
#g++ -o gemcocoawindow.pd_darwin -I/Applications/Pd-0.42-4.app/Contents/Resources/src/ -I../ -bundle -undefined dynamic_lookup -framework Cocoa -framework AGL -framework OpenGL -lstdc++ -ldl -lz -lm gemcocoawindow.mm	
