
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

EXTRA_DIST = filmAVI.sln filmAVI.vcproj

pkglib_LTLIBRARIES= gem_filmAVI.la

gem_filmAVI_la_CXXFLAGS =
gem_filmAVI_la_LDFLAGS  = -module -avoid-version -shared
gem_filmAVI_la_LIBADD   =

# RTE
gem_filmAVI_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmAVI_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
## none

gem_filmAVI_la_SOURCES  = filmAVI.cpp filmAVI.h

