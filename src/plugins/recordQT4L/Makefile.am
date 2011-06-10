
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_recordQT4L.la

gem_recordQT4L_la_CXXFLAGS =
gem_recordQT4L_la_LDFLAGS  = -module -avoid-version -shared
gem_recordQT4L_la_LIBADD   =

# RTE
gem_recordQT4L_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_recordQT4L_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_recordQT4L_la_CXXFLAGS += @GEM_LIB_LIBQUICKTIME_CFLAGS@  @GEM_LIB_LQT_CFLAGS@
gem_recordQT4L_la_LIBADD   += @GEM_LIB_LIBQUICKTIME_LIBS@  @GEM_LIB_LQT_LIBS@

gem_recordQT4L_la_SOURCES= recordQT4L.cpp recordQT4L.h

