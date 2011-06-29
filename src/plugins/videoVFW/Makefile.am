ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

EXTRA_DIST=videoVFW.sln videoVFW.vcproj

pkglib_LTLIBRARIES= gem_videoVFW.la

gem_videoVFW_la_CXXFLAGS =
gem_videoVFW_la_LDFLAGS  = -module -avoid-version -shared
gem_videoVFW_la_LIBADD   =

# RTE
gem_videoVFW_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_videoVFW_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_videoVFW_la_CXXFLAGS += @GEM_LIB_VFW32_CFLAGS@
gem_videoVFW_la_LIBADD   += @GEM_LIB_VFW32_LIBS@

gem_videoVFW_la_SOURCES= videoVFW.cpp videoVFW.h

