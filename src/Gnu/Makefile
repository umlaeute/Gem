# Generated automatically from Makefile.in by configure.
#use "configure" to generate a Makefile out of this

CXXLINK = $(CXX) $(CXXFLAGS) -o $@
LDFLAGS =  -L/usr/X11R6/lib -L../../../GemLibs/glm -L../../../GemLibs/particle -L../../../GemLibs/liborb 
LDADD = -shared
LIBS = -lquicktime -lmpeg3 -lorb -lparticle -lglm -lglib -lgltt -ljpeg -ltiff -lpng -lglut -lGLU -lGL -lz -lm -Wl,-shared -Wl,-export-dynamic -lXxf86vm -lXext -lX11 
INCLUDES = -I/usr/include/ -I../../../GemLibs/liborb/ -I../../../GemLibs/particle/ -I../../../GemLibs/glm/ -I/usr/include/gltt/ -I/usr/local/include/ 

PDLIBDIR = /usr/local/lib/pd
DOCBASE = /share/doc

SYSTEMDOCS = $(DESTDIR)/usr/local/share/doc/gem

TARG_DIR = ../Manips  ../Particles ../Base ../MarkEx ../Pixes ../Controls ../Nongeos  ../Geos ../TV ../openGL ../yuv

OBJECTS = ../Manips/*.o \
         ../Particles/*.o \
         ../Base/*.o \
         ../MarkEx/*.o \
         ../Pixes/*.o \
         ../Controls/*.o \
         ../Nongeos/*.o \
	 ../TV/*.o \
         ../Geos/*.o \
	 ../openGL/*.o \
	 ../yuv/*.o
#OBJECTS = ../Objects/*.o


all: subdirs Gem.pd_linux

cleangem:
	@rm -f Gem.pd_linux

Gem.pd_linux: cleangem $(Gem_pd_DEPENDENCIES)
	$(CXXLINK) $(LDFLAGS) $(OBJECTS) $(LDADD) $(LIBS) $(INCLUDES)
	strip --strip-unneeded Gem.pd_linux
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

	ln -fs $SYSTEMDOCS $(DESTDIR)/$(PDLIBDIR)/doc/
#	ln -fs ../../../$(DOCBASE)/gem $(DESTDIR)/$(PDLIBDIR)/doc/

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
