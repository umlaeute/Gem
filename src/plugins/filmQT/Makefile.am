
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmQT.la
gem_filmQT_la_SOURCES= filmQT.cpp filmQT.h
gem_filmQT_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_FRAMEWORK_QUICKTIME@
gem_filmQT_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_FRAMEWORK_QUICKTIME@

