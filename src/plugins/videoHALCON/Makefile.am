
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoHALCON.la

gem_videoHALCON_la_CXXFLAGS =
gem_videoHALCON_la_LDFLAGS  = -module -avoid-version -shared
gem_videoHALCON_la_LIBADD   =

# RTE
gem_videoHALCON_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_videoHALCON_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_videoHALCON_la_CXXFLAGS += @GEM_HALCON_CXXFLAGS@
gem_videoHALCON_la_LIBADD   += @GEM_HALCON_LIBS@

gem_videoHALCON_la_SOURCES= videoHALCON.cpp videoHALCON.h

