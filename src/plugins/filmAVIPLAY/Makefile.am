
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmAVIPLAY.la

gem_filmAVIPLAY_la_CXXFLAGS =
gem_filmAVIPLAY_la_LDFLAGS  = -module -avoid-version -shared
gem_filmAVIPLAY_la_LIBADD   =

# RTE
gem_filmAVIPLAY_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmAVIPLAY_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_filmAVIPLAY_la_CXXFLAGS += @GEM_LIB_AVIFILE_CFLAGS@
gem_filmAVIPLAY_la_LIBADD   += @GEM_LIB_AVIFILE_LIBS@

gem_filmAVIPLAY_la_SOURCES= filmAVIPLAY.cpp filmAVIPLAY.h
