default: all
SRCDIR=.
include Make.include

BUILDDIR = ../

# all objects that have been build
OBJECTS = $(GEM_TARGETS:%=%/*.o)

.PHONY: default all subdirs $(GEM_TARGETS) clean distclean install installdocs

all: subdirs Gem.$(EXT) strip

Gem.$(EXT): subdirs
	$(CXXLINK) $(GEM_LDFLAGS) $(OBJECTS) $(GEM_LIBS) $(X_LIBS)

strip:
	-$(STRIP) $(STRIP_UNNEEDED) Gem.$(EXT)

## handling of subdirs as proposed by the GNU-make documentation

subdirs: $(GEM_TARGETS)

$(GEM_TARGETS):
	${MAKE} -C $@ -f $(BUILDDIR)/Makefile.subdir -j4

clean:
	@for d in ${GEM_TARGETS}; do \
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d -f $(BUILDDIR)/Makefile.subdir clean; \
	done
	-rm Gem.$(EXT) *~

distclean:
	@for d in ${GEM_TARGETS}; do \
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d -f $(BUILDDIR)/Makefile.subdir distclean; \
	done
	-rm Gem.$(EXT) *~
	-rm config.cache config.log config.status Make.config

install: installdocs
	$(INSTALL_DATA) -D Gem.$(EXT) $(DESTDIR)/$(PDLIBDIR)/extra/Gem.$(EXT)

installdocs:
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/doc/gem
	cp -r ./../examples $(DESTDIR)/$(PDLIBDIR)/doc/gem
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)
	$(INSTALL_DATA) ./../help/*.* $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)


Make.config: Make.config.in configure
	./configure

configure: configure.ac aclocal.m4
	autoconf

aclocal.m4: acinclude.m4
	aclocal
