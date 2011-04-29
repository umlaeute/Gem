EXTRANAME=pix_mano
default: ${EXTRANAME}

SRCDIR=../..

-include $(SRCDIR)/Make.config

EXTRALIBDIR=$(GEMLIBDIR)/../$(EXTRANAME)

INSTALLdir=pd/extra/Gem
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = ${INSTALL} -m 644

CXXLINK = $(CXX) -o $@

SOURCES=$(sort $(filter %.cpp, $(wildcard *.cpp)))
OBJECTS = $(SOURCES:.cpp=.o)
PD_OBJECTS = $(SOURCES:.cpp=.$(EXT))

PD_PATCHES=$(sort $(filter %.pd, $(wildcard *.pd)))

${EXTRANAME}: $(PD_OBJECTS)

%.o: %.cpp
	$(CXX) -c $(GEM_EXTRA_CXXFLAGS) $(GEM_ARCH_CXXFLAGS) $(GEM_CXXFLAGS) -I$(SRCDIR) $< -o $@


%.$(EXT): %.o
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $^ $(GEM_LIBS) $(X_LIBS) 

clean:
	-rm -f ${OBJECTS} ${PD_OBJECTS}

distclean:clean
	-rm *~

test:
	-echo $(PD_OBJECTS)
	-echo $(EXT)

install: ${PD_OBJECTS} $(PD_PATCHES)
	$(INSTALL) -d $(DESTDIR)/$(EXTRALIBDIR)
	$(INSTALL_DATA) -D $^ $(DESTDIR)/$(EXTRALIBDIR)