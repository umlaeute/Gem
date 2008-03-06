default: all
SRCDIR=.
include Make.include

BUILDDIR = ..

# all objects that have been build
#OBJECTS = $(GEM_TARGETS:%=%/*.o)
OBJECTS = $(OBJDIR)*.o

.PHONY: default all subdirs $(GEM_TARGETS) clean distclean install installdocs installabs strip config

all: subdirs strip

Gem.$(EXT): subdirs 
	$(CXXLINK) $(GEM_LDFLAGS) $(OBJECTS) $(GEM_LIBS) $(X_LIBS)

strip: Gem.$(EXT)
	-$(STRIP) $(STRIP_UNNEEDED) Gem.$(EXT)

Objects:
	mkdir Objects

## handling of subdirs as proposed by the GNU-make documentation
subdirs: Objects $(GEM_TARGETS)

$(GEM_TARGETS):
	${MAKE} -C $@ 

clean:
	@for d in ${GEM_TARGETS}; do \
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d clean; \
	done
	-rm -f Gem.$(EXT) *~
	-rm -f $(OBJDIR)*.o

distclean:
	@for d in ${GEM_TARGETS}; do \
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d distclean; \
	done
	-rm -f Gem.$(EXT) *~
	-rm -f config.cache config.log config.status Make.config

install: installdocs installabs
	$(INSTALL_DATA) -D Gem.$(EXT) $(DESTDIR)/$(PDLIBDIR)/extra/Gem/Gem.$(EXT)

installabs:
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/extra/Gem
	$(INSTALL_DATA) ./../abstractions/*.pd $(DESTDIR)/$(PDLIBDIR)/extra/Gem

installdocs:
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/doc/gem
	cp -r ./../examples $(DESTDIR)/$(PDLIBDIR)/doc/gem
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)
	$(INSTALL_DATA) ./../help/*.* $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)


ifeq (,$(findstring clean, $(MAKECMDGOALS)))
## hacks again!
ifeq (,$(findstring configure, $(MAKECMDGOALS)))
ifeq (,$(findstring aclocal.m4, $(MAKECMDGOALS)))
Make.config: Make.config.in configure $(GEM_CONFIGFILE)
	./configure ${CONFIGUREFLAGS}
endif
endif

config: Make.config

configure: configure.ac aclocal.m4
	autoconf

aclocal.m4: acinclude.m4
	aclocal
endif
