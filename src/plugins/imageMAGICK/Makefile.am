
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageMAGICK.la

gem_imageMAGICK_la_CXXFLAGS =
gem_imageMAGICK_la_LDFLAGS  = -module -avoid-version -shared
gem_imageMAGICK_la_LIBADD   =

# RTE
gem_imageMAGICK_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_imageMAGICK_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_imageMAGICK_la_CXXFLAGS += @GEM_LIB_IMAGEMAGICK___CFLAGS@
gem_imageMAGICK_la_LIBADD   += @GEM_LIB_IMAGEMAGICK___LIBS@

gem_imageMAGICK_la_SOURCES= imageMAGICK.cpp imageMAGICK.h

