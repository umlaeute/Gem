
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoDC1394.la
gem_videoDC1394_la_SOURCES= videoDC1394.cpp videoDC1394.h
gem_videoDC1394_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_IEEE1394_CFLAGS@
gem_videoDC1394_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@  @GEM_LIB_DC1394_LIBS@

