
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoHALCON.la
gem_videoHALCON_la_SOURCES= videoHALCON.cpp videoHALCON.h
gem_videoHALCON_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBHALCON_CFLAGS@
gem_videoHALCON_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_LIBHALCON_LIBS@


