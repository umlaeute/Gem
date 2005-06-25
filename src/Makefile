include Make.config

BUILDDIR = ../


OBJECTS = ./Objects/*.o

all: subdirs Gem.pd_linux

Gem.static:
#	-ld  $(OBJECTS) $(LIBS) $(X_LIBS) /usr/bin/pd

Gem.pd_linux: Gem.static $(Gem_pd_DEPENDENCIES)
	$(CXXLINK) $(SHARED) $(OBJECTS) $(LIBS) $(X_LIBS)
	$(STRIP) Gem.pd_linux

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
	-rm Gem.pd_linux *~ Gem.static


install: installdocs
	install -m644 -D Gem.pd_linux $(DESTDIR)/$(PDLIBDIR)/extra/Gem.pd_linux


installdocs:
	install -d $(DESTDIR)/$(PDLIBDIR)/doc/gem
	cp -r ./../examples $(DESTDIR)/$(PDLIBDIR)/doc/gem
       
	install -d $(DESTDIR)/$(PDLIBDIR)/doc/5.reference/Gem
	install -m644 ./../help/*.* $(DESTDIR)/$(PDLIBDIR)/doc/5.reference/Gem
	ln -s Gem $(DESTDIR)/$(PDLIBDIR)/doc/5.reference/help-Gem

distclean: clean
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    cd $$d ; ${MAKE} -f $(BUILDDIR)/Makefile.subdir distclean; \
	done
	-rm config.cache config.log config.status Make.config

Make.config: Make.config.in configure
	./configure

configure: configure.ac
	#autoheader && aclocal && autoconf

