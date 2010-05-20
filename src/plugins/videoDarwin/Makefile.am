
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoDarwin.la
gem_videoDarwin_la_SOURCES= videoDarwin.cpp videoDarwin.h
gem_videoDarwin_la_CXXFLAGS = @GEM_RTE_CFLAGS@ 
gem_videoDarwin_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_FRAMEWORK_QUICKTIME@ @GEM_FRAMEWORK_CARBON@


