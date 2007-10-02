default: all
SRCDIR=.
include Make.include

BUILDDIR = ..

# all objects that have been build
OBJECTS = $(GEM_TARGETS:%=%/*.o)

.PHONY: default all subdirs $(GEM_TARGETS) clean distclean install installdocs

all: subdirs Gem.$(EXT) strip

Gem.$(EXT): subdirs
	$(CXXLINK) $(GEM_LDFLAGS) $(OBJECTS) $(GEM_LIBS) $(X_LIBS)

strip: Gem.$(EXT)
	-$(STRIP) $(STRIP_UNNEEDED) Gem.$(EXT)

## handling of subdirs as proposed by the GNU-make documentation

subdirs: $(GEM_TARGETS)

$(GEM_TARGETS):
	${MAKE} -C $@ 

clean:
	@for d in ${GEM_TARGETS}; do \
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d clean; \
	done
	-rm -f Gem.$(EXT) *~

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
Make.config: Make.config.in configure $(GEM_CONFIGFILE)
	./configure ${CONFIGUREFLAGS}

configure: configure.ac aclocal.m4
	autoconf

aclocal.m4: acinclude.m4
	aclocal
endif
