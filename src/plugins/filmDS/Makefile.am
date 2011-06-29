
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

EXTRA_DIST = filmDS.sln filmDS.vcproj

pkglib_LTLIBRARIES= gem_filmDS.la

gem_filmDS_la_CXXFLAGS =
gem_filmDS_la_LDFLAGS  = -module -avoid-version -shared
gem_filmDS_la_LIBADD   =

# RTE
gem_filmDS_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmDS_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
# none

### SOURCES
gem_filmDS_la_SOURCES= filmDS.cpp filmDS.h

