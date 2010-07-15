
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_recordV4L2.la
gem_recordV4L2_la_SOURCES= recordV4L2.cpp recordV4L2.h
gem_recordV4L2_la_CXXFLAGS = @GEM_RTE_CFLAGS@
gem_recordV4L2_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ 

