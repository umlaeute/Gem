
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmMPEG3.la
gem_filmMPEG3_la_SOURCES= filmMPEG3.cpp filmMPEG3.h
gem_filmMPEG3_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBMPEG3_CFLAGS@
gem_filmMPEG3_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_LIBMPEG3_LIBS@

