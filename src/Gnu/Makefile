# Generated automatically from Makefile.in by configure.


CXXLINK = $(CXX) $(CXXFLAGS) -o $@
LDFLAGS =  -L/usr/X11R6/lib -L../../../GemLibs/glm  -L../../../GemLibs/particle  -L../../../GemLibs/liborb 
LDADD = -shared
LIBS = -L/usr/lib -lglib -lz -lgltt -lttf -ljpeg -ltiff -lmpeg3 -lmpeg -lquicktime -lglib -lpng -lglut -lGLU -lGL -lm -Wl,-shared -Wl,-export-dynamic -lXxf86vm -lXext -lX11  -lglm -lparticle -lorb
INCLUDES = -I/usr/include/glib-1.2 -I/usr/lib/glib/include -I/usr/include/gltt -I/usr/local/include -I..   -I../../../GemLibs/glm -I../../../GemLibs/liborb -I../../../GemLibs/particle
GEMLIBS = ../../../GemLibs

PREFIX = /usr/local
PDLIBDIR = $(PREFIX)/lib/pd
DOCBASE = /share/doc

SYSTEMDOCS = $(DESTDIR)/$(PREFIX)/$(DOCBASE)/gem

TARG_DIR = ../Manips  ../Particles ../Base ../MarkEx ../Pixes ../Controls ../Nongeos  ../Geos ../TV

OBJECTS = ../Manips/*.o \
         ../Particles/*.o \
         ../Base/*.o \
         ../MarkEx/*.o \
         ../Pixes/*.o \
         ../Controls/*.o \
         ../Nongeos/*.o \
	 ../TV/*.o \
         ../Geos/*.o \
#OBJECTS = ../Objects/*.o


all: subdirs Gem.pd_linux

cleangem:
	@rm -f Gem.pd_linux

Gem.pd_linux: cleangem $(Gem_pd_DEPENDENCIES)
	$(CXXLINK) $(LDFLAGS) $(OBJECTS) $(LDADD) $(LIBS) $(INCLUDES)
	cp Gem.pd_linux ../../


subdirs:
	for d in ${TARG_DIR}; do \
	    echo ""\
	    echo "----------$$d----------"; \
	    cd $$d && ${MAKE} || exit -1; \
	done


depend:
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    cd $$d ; ${MAKE} depend; cd ../Gnu; \
	done


clean:
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    cd $$d ; ${MAKE} clean;cd ../Gnu; \
	done
	-rm Gem.pd_linux ../../Gem.pd_linux *~


install: installdocs uncvsdocs
	install -m644 -D ../../Gem.pd_linux $(DESTDIR)/$(PDLIBDIR)/externs/Gem.pd_linux


installdocs:
	install -d $(SYSTEMDOCS)
	install -m644 ../../doc/*.* $(SYSTEMDOCS)

	install -d $(DESTDIR)/$(PDLIBDIR)/doc/5.reference/Gem
	install -m644 ../../help/*.* $(DESTDIR)/$(PDLIBDIR)/doc/5.reference/Gem

	cp -r ../../examples $(SYSTEMDOCS)
	cp -r ../../manual $(SYSTEMDOCS)

	ln -fs ../../../$(DOCBASE)/gem $(DESTDIR)/$(PDLIBDIR)/doc/

uncvsdocs:
	touch $(SYSTEMDOCS)/CVS
	rm -rf `find $(SYSTEMDOCS) -name *CVS*`

distclean: clean
	@for d in ${TARG_DIR}; do \
	    echo "\n----------$$d----------"; \
	    cd $$d ; ${MAKE} distclean;cd ../Gnu; \
	done
	-rm config.cache config.log config.status Makefile
	./makeauxlibs clean
	cp Makefile.clean Makefile


