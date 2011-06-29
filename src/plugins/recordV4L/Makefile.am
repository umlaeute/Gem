
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

pkglib_LTLIBRARIES= gem_recordV4L.la

gem_recordV4L_la_CXXFLAGS =
gem_recordV4L_la_LDFLAGS  = -module -avoid-version -shared
gem_recordV4L_la_LIBADD   =

# RTE
gem_recordV4L_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_recordV4L_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_recordV4L_la_CXXFLAGS += @GEM_LIB_LIBV4L1_CFLAGS@
gem_recordV4L_la_LIBADD   += @GEM_LIB_LIBV4L1_LIBS@

gem_recordV4L_la_SOURCES= recordV4L.cpp recordV4L.h

