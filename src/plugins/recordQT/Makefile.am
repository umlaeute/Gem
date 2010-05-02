
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_recordQT.la
gem_recordQT_la_SOURCES= recordQT.cpp recordQT.h
gem_recordQT_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_FRAMEWORK_QUICKTIME@
gem_recordQT_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_FRAMEWORK_QUICKTIME@

