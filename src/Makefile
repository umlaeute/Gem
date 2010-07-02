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

Gem.$(EXT): version.h subdirs 
	$(CXXLINK) $(GEM_EXTRA_LDFLAGS)  $(GEM_ARCH_LDFLAGS) $(GEM_LDFLAGS) $(OBJECTS) $(GEM_LIBS) $(X_LIBS)

Gem: Gem.$(EXT)

version.h: version.h.in
	${SHELL} ./gemversion.sh version.h
	

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
	$(INSTALL_DATA) -D Gem.$(EXT) $(DESTDIR)/$(GEMLIBDIR)/Gem.$(EXT)

installplugins: PLUGINS
	${MAKE} -C plugins install

installextra: extra
	${MAKE} -C extra install

installabs:
	$(INSTALL) -d $(DESTDIR)/$(GEMDLIBDIR)
	$(INSTALL_DATA) ./../abstractions/*.pd $(DESTDIR)/$(GEMLIBDIR)

installdocs:
	$(INSTALL) -d $(DESTDIR)/$(GEMLIBDIR)
	$(INSTALL_DATA) ./../help/*.* $(DESTDIR)/$(GEMLIBDIR)
	$(INSTALL) -d $(DESTDIR)/$(GEMLIBDIR)
	cp -r ./../examples $(DESTDIR)/$(GEMLIBDIR)

