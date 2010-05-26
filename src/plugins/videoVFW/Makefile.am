
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoVFW.la
gem_videoVFW_la_SOURCES= videoVFW.cpp videoVFW.h
gem_videoVFW_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_VFW32_CFLAGS@
gem_videoVFW_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_VFW32_LIBS@

