
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoDS.la
gem_videoDS_la_SOURCES= videoDS.cpp videoDS.h DSgrabber.cpp DSgrabber.h
gem_videoDS_la_CXXFLAGS = @GEM_RTE_CFLAGS@ 
wem_videoDS_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ 

