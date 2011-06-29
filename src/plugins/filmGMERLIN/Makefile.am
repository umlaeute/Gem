
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_filmGMERLIN.la

gem_filmGMERLIN_la_CXXFLAGS =
gem_filmGMERLIN_la_LDFLAGS  = -module -avoid-version -shared
gem_filmGMERLIN_la_LIBADD   =

# RTE
gem_filmGMERLIN_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmGMERLIN_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_filmGMERLIN_la_CXXFLAGS += @GEM_LIB_GMERLIN_AVDEC_CFLAGS@
gem_filmGMERLIN_la_LIBADD   += @GEM_LIB_GMERLIN_AVDEC_LIBS@

### SOURCES
gem_filmGMERLIN_la_SOURCES= filmGMERLIN.cpp filmGMERLIN.h

