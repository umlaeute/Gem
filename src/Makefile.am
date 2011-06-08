## notes:
# LIBADD vs LDADD
#   LIBADD: ADDitional LIBraries
#   LDADD : ADDitional Linker Objects
# also::  LIBADD (for other libraries) or LDADD (for final executables).


AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS=Base RTE plugins 

SUBDIRS+= Controls Geos Manips Nongeos openGL Particles Pixes

lib_LTLIBRARIES = Gem.la

## make sure we use a c++-linker
###  hmmm, nodist_EXTRA seems to not work...
#nodist_EXTRA_Gem_la_SOURCES = dummy.cxx
#EXTRA_Gem_la_SOURCES=dummy.cxx
nodist_Gem_la_SOURCES = dummy.cxx


Gem_la_CXXFLAGS =
Gem_la_LDFLAGS = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@

Gem_la_LDFLAGS += @ARCH_FLAGS@

# LIBADD or LDADD for convenience libraries?
#Gem_la_LIBADD = Base/libBase.la RTE/libRTE.la @GEM_RTE_LIBS@
Gem_la_LIBADD = Base/libBase.la RTE/libRTE.la plugins/libplugins.la

Gem_la_LIBADD += Controls/libControls.la
Gem_la_LIBADD += Geos/libGeos.la
Gem_la_LIBADD += Manips/libManips.la
Gem_la_LIBADD += Nongeos/libNongeos.la
Gem_la_LIBADD += openGL/libopenGL.la
Gem_la_LIBADD += Particles/libParticles.la
Gem_la_LIBADD += Pixes/libPixes.la

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
