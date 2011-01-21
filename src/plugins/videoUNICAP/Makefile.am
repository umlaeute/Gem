
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoUNICAP.la
gem_videoUNICAP_la_SOURCES= videoUNICAP.cpp videoUNICAP.h
gem_videoUNICAP_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBUNICAP_CFLAGS@ 
gem_videoUNICAP_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@  @GEM_LIB_LIBUNICAP_LIBS@

