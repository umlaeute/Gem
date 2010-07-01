
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoAVT.la
gem_videoAVT_la_SOURCES= videoAVT.cpp videoAVT.h
gem_videoAVT_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBPVAPI_CFLAGS@
gem_videoAVT_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_LIB_LIBPVAPI_LIBS@


