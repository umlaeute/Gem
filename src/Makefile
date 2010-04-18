default: all
SRCDIR=.
include Make.include

BUILDDIR = ..

# all objects that have been build
#OBJECTS = $(GEM_TARGETS:%=%/*.o)
OBJECTS = $(OBJDIR)*.o

.PHONY: default all subdirs clean distclean install installdocs installabs installplugins strip config \
	$(GEM_TARGETS) Gem extra PLUGINS

all: Gem.$(EXT) extra PLUGINS

PLUGINS:
	${MAKE} -C plugins plugins

Gem.$(EXT): subdirs 
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(OBJECTS) $(GEM_LIBS) $(X_LIBS)

Gem: Gem.$(EXT)

strip: Gem.$(EXT)
	-$(STRIP) $(STRIP_UNNEEDED) Gem.$(EXT)

Objects:
	mkdir Objects

## handling of subdirs as proposed by the GNU-make documentation
subdirs: Objects $(GEM_TARGETS)

$(GEM_TARGETS):
	${MAKE} -C $@ 

extra:
	${MAKE} -k -C extra

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

install: installdocs installabs installplugins installextra
	$(INSTALL_DATA) -D Gem.$(EXT) $(DESTDIR)/$(PDLIBDIR)/extra/Gem/Gem.$(EXT)

installplugins: PLUGINS
	make -C plugins install

installextra: extra
	make -C extra install

installabs:
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/extra/Gem
	$(INSTALL_DATA) ./../abstractions/*.pd $(DESTDIR)/$(PDLIBDIR)/extra/Gem

installdocs:
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)
	$(INSTALL_DATA) ./../help/*.* $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)
	$(INSTALL) -d $(DESTDIR)/$(PDLIBDIR)/doc/gem
	cp -r ./../examples $(DESTDIR)/$(PDLIBDIR)/doc/gem

