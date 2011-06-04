
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageMAGICK.la
gem_imageMAGICK_la_SOURCES= imageMAGICK.cpp imageMAGICK.h
gem_imageMAGICK_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_IMAGEMAGICK___CFLAGS@
gem_imageMAGICK_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_IMAGEMAGICK___LIBS@

