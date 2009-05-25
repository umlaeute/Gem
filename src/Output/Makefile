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
