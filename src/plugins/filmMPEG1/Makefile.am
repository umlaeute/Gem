
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmMPEG1.la
gem_filmMPEG1_la_SOURCES= filmMPEG1.cpp filmMPEG1.h
gem_filmMPEG1_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_MPEG_CFLAGS@
gem_filmMPEG1_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_MPEG_LIBS@

