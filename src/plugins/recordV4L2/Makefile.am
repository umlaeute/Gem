
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_recordV4L2.la

gem_recordV4L2_la_CXXFLAGS =
gem_recordV4L2_la_LDFLAGS  = -module -avoid-version -shared
gem_recordV4L2_la_LIBADD   =

# RTE
gem_recordV4L2_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_recordV4L2_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_recordV4L2_la_CXXFLAGS += @GEM_LIB_LIBV4L2_CFLAGS@ 
gem_recordV4L2_la_LIBADD   += @GEM_LIB_LIBV4L2_LIBS@

### SOURCES
gem_recordV4L2_la_SOURCES= recordV4L2.cpp recordV4L2.h

