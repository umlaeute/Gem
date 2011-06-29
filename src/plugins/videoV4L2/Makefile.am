
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

pkglib_LTLIBRARIES= gem_videoV4L2.la

gem_videoV4L2_la_CXXFLAGS =
gem_videoV4L2_la_LDFLAGS  = -module -avoid-version -shared
gem_videoV4L2_la_LIBADD   =

# RTE
gem_videoV4L2_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_videoV4L2_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_videoV4L2_la_CXXFLAGS += @GEM_LIB_LIBV4L2_CFLAGS@ 
gem_videoV4L2_la_LIBADD   += @GEM_LIB_LIBV4L2_LIBS@

gem_videoV4L2_la_SOURCES= videoV4L2.cpp videoV4L2.h

