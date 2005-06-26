include Make.config

BUILDDIR = ../

OBJECTS = ./Objects/*.o

all: subdirs Gem.$(EXT)

Gem.static:
#	-ld $(LDFLAGS) $(OBJECTS) $(LIBS) $(X_LIBS) /usr/bin/pd

Gem.$(EXT): Gem.static $(Gem_pd_DEPENDENCIES)
	$(CXXLINK) $(LDFLAGS) $(OBJECTS) $(LIBS) $(X_LIBS)
	$(STRIP) Gem.$(EXT)

subdirs:
	for d in ${TARG_DIR}; do \
	    echo ""\
	    echo "----------$$d----------"; \
	    ${MAKE} -C $$d -f $(BUILDDIR)/Makefile.subdir -j4|| exit -1; \
	done


depend:
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    ${MAKE} -f $(BUILDDIR)/Makefile.subdir -C $$d depend; \
	done


clean:
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    ${MAKE} -f $(BUILDDIR)/Makefile.subdir -C $$d clean; \
	done
	-rm Gem.$(EXT) *~ Gem.static


install: installdocs
	install -m644 -D Gem.$(EXT) $(DESTDIR)/$(PDLIBDIR)/extra/Gem.$(EXT)


installdocs:
	install -d $(DESTDIR)/$(PDLIBDIR)/doc/gem
	cp -r ./../examples $(DESTDIR)/$(PDLIBDIR)/doc/gem
	install -d $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)
	install -m644 ./../help/*.* $(DESTDIR)/$(PDLIBDIR)/$(PDREFPATH)

distclean: clean
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    cd $$d ; ${MAKE} -f $(BUILDDIR)/Makefile.subdir distclean; \
	done
	-rm config.cache config.log config.status Make.config

Make.config: Make.config.in configure
	./configure

configure: configure.ac
	aclocal && autoconf

