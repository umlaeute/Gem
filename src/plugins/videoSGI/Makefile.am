
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoSGI.la

gem_videoSGI_la_CXXFLAGS =
gem_videoSGI_la_LDFLAGS  = -module -avoid-version -shared
gem_videoSGI_la_LIBADD   =

# RTE
gem_videoSGI_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_videoSGI_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
## none

gem_videoSGI_la_SOURCES= videoSGI.cpp videoSGI.h

