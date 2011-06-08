
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageJPEG.la
gem_imageJPEG_la_SOURCES= imageJPEG.cpp imageJPEG.h
gem_imageJPEG_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_JPEG_CFLAGS@
gem_imageJPEG_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_JPEG_LIBS@

