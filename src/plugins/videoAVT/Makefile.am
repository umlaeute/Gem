
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoAVT.la

gem_videoAVT_la_CXXFLAGS =
gem_videoAVT_la_LDFLAGS  = -module -avoid-version -shared
gem_videoAVT_la_LIBADD   =

# RTE
gem_videoAVT_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_videoAVT_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_videoAVT_la_CXXFLAGS += @GEM_LIB_LIBPVAPI_CFLAGS@
gem_videoAVT_la_LIBADD   += @GEM_LIB_LIBPVAPI_LIBS@

gem_videoAVT_la_SOURCES= videoAVT.cpp videoAVT.h

