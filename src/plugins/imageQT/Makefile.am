
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageQT.la
gem_imageQT_la_SOURCES= imageQT.cpp imageQT.h
gem_imageQT_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_FRAMEWORK_QUICKTIME@
gem_imageQT_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_FRAMEWORK_QUICKTIME@

