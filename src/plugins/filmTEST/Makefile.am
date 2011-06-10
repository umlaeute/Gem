
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_filmTEST.la

gem_filmTEST_la_CXXFLAGS =
gem_filmTEST_la_LDFLAGS  = -module -avoid-version -shared
gem_filmTEST_la_LIBADD   =

# RTE
gem_filmTEST_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmTEST_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_filmTEST_la_CXXFLAGS +=
gem_filmTEST_la_LIBADD   +=

gem_filmTEST_la_SOURCES= filmTEST.cpp filmTEST.h

