
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageTIFF.la
gem_imageTIFF_la_SOURCES= imageTIFF.cpp imageTIFF.h
gem_imageTIFF_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_TIFF_CFLAGS@
gem_imageTIFF_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_TIFF_LIBS@

