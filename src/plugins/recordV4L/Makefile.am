
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_recordV4L.la
gem_recordV4L_la_SOURCES= recordV4L.cpp recordV4L.h
gem_recordV4L_la_CXXFLAGS = @GEM_RTE_CFLAGS@
gem_recordV4L_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ 

