
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_filmQT4L.la

gem_filmQT4L_la_CXXFLAGS =
gem_filmQT4L_la_LDFLAGS  = -module -avoid-version -shared
gem_filmQT4L_la_LIBADD   =

# RTE
gem_filmQT4L_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmQT4L_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_filmQT4L_la_CXXFLAGS += @GEM_LIB_LIBQUICKTIME_CFLAGS@ @GEM_LIB_LQT_CFLAGS@
gem_filmQT4L_la_LIBADD   += @GEM_LIB_LIBQUICKTIME_LIBS@   @GEM_LIB_LQT_LIBS@

gem_filmQT4L_la_SOURCES= filmQT4L.cpp filmQT4L.h

