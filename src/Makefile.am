## notes:
# LIBADD vs LDADD
#   LIBADD: ADDitional LIBraries
#   LDADD : ADDitional Linker Objects
# also::  LIBADD (for other libraries) or LDADD (for final executables).

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

# make sure -lstdc++ is kept first
## http://wiki.fifengine.de/Segfault_in_cxa_allocate_exception#Workaround_.231
## for more info see configure.ac
AM_LIBTOOLFLAGS = --preserve-dup-deps

DISTCLEANFILES =

SUBDIRS=Gem RTE Utils Base plugins
pkglib_LTLIBRARIES = Gem.la

## make sure we use a c++-linker
Gem_la_SOURCES = Version.cpp

## some default flags
Gem_la_CXXFLAGS = -DHAVE_VERSION_H -DGEM_INTERNAL $(GEM_CPPFLAGS)
Gem_la_LDFLAGS  = -module -avoid-version -shared -shrext .$(GEM_RTE_EXTENSION)
if WINDOWS
Gem_la_LDFLAGS += -no-undefined
endif
Gem_la_LIBADD   =
sublibs =


# LIBADD or LDADD for convenience libraries?
sublibs += Gem/libGem.la RTE/libRTE.la Utils/libUtils.la Base/libBase.la plugins/libplugins.la

## Output: get's build directly from $(topsrcdir)
#if DISABLED
#SUBDIRS+=Output
#endif

if TARGET_CONTROLS
SUBDIRS+=Controls
sublibs += Controls/libControls.la
endif
if TARGET_GEOS
SUBDIRS+=Geos
sublibs += Geos/libGeos.la
endif
if TARGET_MANIPS
SUBDIRS+=Manips
sublibs += Manips/libManips.la
endif
if TARGET_NONGEOS
SUBDIRS+=Nongeos
sublibs += Nongeos/libNongeos.la
endif
if TARGET_OPENGL
SUBDIRS+=openGL
sublibs += openGL/libopenGL.la
endif
if TARGET_PARTICLES
SUBDIRS+=Particles
sublibs += Particles/libParticles.la
endif
if TARGET_PIXES
SUBDIRS+=Pixes
sublibs += Pixes/libPixes.la
endif
if TARGET_VERTEX
SUBDIRS+=Vertex
sublibs += Vertex/libVertex.la
endif

Gem_la_LIBADD += $(sublibs)

SUBDIRS+=deprecated

# RTE flags
Gem_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
Gem_la_LIBADD   += $(GEM_RTE_LIBS)
Gem_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

# openGL
Gem_la_CXXFLAGS += @GL_CFLAGS@ @GLU_CFLAGS@ @AGL_CFLAGS@
Gem_la_LIBADD   += @GL_LIBS@   @GLU_LIBS@   @AGL_LIBS@
Gem_la_LDFLAGS  += @GL_LDFLAGS@ @GLU_LDFLAGS@
# and glew
if ENABLE_MULTICONTEXT
Gem_la_CXXFLAGS += $(GEM_LIB_GLEWMX_CFLAGS)
Gem_la_LIBADD   += $(GEM_LIB_GLEWMX_LIBS)
else !ENABLE_MULTICONTEXT
Gem_la_CXXFLAGS += $(GEM_LIB_GLEW_CFLAGS)
Gem_la_LIBADD   += $(GEM_LIB_GLEW_LIBS)
endif !ENABLE_MULTICONTEXT
Gem_la_LDFLAGS  +=

if MINGW
Gem_la_LDFLAGS += -static-libgcc -static-libstdc++ --static
endif


libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


## version stuff
.PHONY: FORCE

FORCE:

Version.cpp: version_current.h

version_current.h: $(srcdir)/version.h.in $(sublibs)
	$(SHELL) $(srcdir)/pkgversion.sh $< > $@

DISTCLEANFILES += version_current.h
CHECKUNUSED_IGNORE = version_current.h
## /version stuff



## uha, this is ugly
$(abs_builddir)/.libs/Gem.$(GEM_RTE_EXTENSION): Gem.la

$(top_builddir)/Gem.$(GEM_RTE_EXTENSION): $(abs_builddir)/.libs/Gem.$(GEM_RTE_EXTENSION)
		rm -f $@ && cd $(top_builddir) && test -e $< && $(LN_S) $< Gem.$(GEM_RTE_EXTENSION) || true

.PHONY: clean-conveniencelink

clean-conveniencelink:
	test -L $(top_builddir)/Gem.$(GEM_RTE_EXTENSION) && rm -f $(top_builddir)/Gem.$(GEM_RTE_EXTENSION) || true


all-local:: $(top_builddir)/Gem.$(GEM_RTE_EXTENSION)

clean-local:: clean-conveniencelink

EXTRA_Gem_la_SOURCES = \
	pkgversion.sh \
	pkgversion.txt \
	version.h.in
