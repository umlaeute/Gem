
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmDarwin.la
gem_filmDarwin_la_SOURCES= filmDarwin.cpp filmDarwin.h
gem_filmDarwin_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_FRAMEWORK_QUICKTIME@ @GEM_FRAMEWORK_CARBON@ 
gem_filmDarwin_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_FRAMEWORK_QUICKTIME@ @GEM_FRAMEWORK_CARBON@ 

