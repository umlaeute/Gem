## notes:
# LIBADD vs LDADD
#   LIBADD: ADDitional LIBraries
#   LDADD : ADDitional Linker Objects
# also::  LIBADD (for other libraries) or LDADD (for final executables).


AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

# make sure -lstdc++ is kept first
AM_LIBTOOLFLAGS = --preserve-dup-deps

SUBDIRS=Base RTE plugins 
lib_LTLIBRARIES = Gem.la

## make sure we use a c++-linker
Gem_la_SOURCES = 
nodist_EXTRA_Gem_la_SOURCES = dummy.cpp

## some default flags
Gem_la_CXXFLAGS =
Gem_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
Gem_la_LIBADD   =


# RTE flags
Gem_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
Gem_la_LIBADD   += @GEM_RTE_LIBS@

Gem_la_LDFLAGS += @GEM_ARCH_LDFLAGS@


# LIBADD or LDADD for convenience libraries?
#Gem_la_LIBADD = Base/libBase.la RTE/libRTE.la @GEM_RTE_LIBS@
Gem_la_LIBADD += Gem/libGem.la RTE/libRTE.la Base/libBase.la plugins/libplugins.la

if TARGET_CONTROLS
SUBDIRS+=Controls
Gem_la_LIBADD += Controls/libControls.la
endif
if TARGET_GEOS
SUBDIRS+=Geos
Gem_la_LIBADD += Geos/libGeos.la
endif
if TARGET_MANIPS
SUBDIRS+=Manips
Gem_la_LIBADD += Manips/libManips.la
endif
if TARGET_NONGEOS
SUBDIRS+=Nongeos
Gem_la_LIBADD += Nongeos/libNongeos.la
endif
if TARGET_OPENGL
SUBDIRS+=openGL
Gem_la_LIBADD += openGL/libopenGL.la
endif
if TARGET_PARTICLES
SUBDIRS+=Particles
Gem_la_LIBADD += Particles/libParticles.la
endif
if TARGET_PIXES
SUBDIRS+=Pixes
Gem_la_LIBADD += Pixes/libPixes.la
endif
if TARGET_VERTEX
SUBDIRS+=Vertex
Gem_la_LIBADD += Vertex/libVertex.la
endif

SUBDIRS+=extra


libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

## uha, this is ugly
Gem.@GEM_RTE_EXTENSION@: Gem.la
	rm -f $@
	test .libs/$@ && $(LN_S) .libs/$@ $@

all-local:: Gem.@GEM_RTE_EXTENSION@
