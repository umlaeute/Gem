
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_recordQT4L.la
gem_recordQT4L_la_SOURCES= recordQT4L.cpp recordQT4L.h
gem_recordQT4L_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBQUICKTIME_CFLAGS@  @GEM_LIB_LQT_CFLAGS@
gem_recordQT4L_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@  @GEM_LIB_LIBQUICKTIME_LIBS@  @GEM_LIB_LQT_LIBS@

