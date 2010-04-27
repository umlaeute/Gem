
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoV4L2.la
gem_videoV4L2_la_SOURCES= videoV4L2.cpp videoV4L2.h
gem_videoV4L2_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_LIB_LIBV4L2_CFLAGS@ 
gem_videoV4L2_la_LDFLAGS = -module @GEM_RTE_LIBS@  @GEM_LIB_LIBV4L2_LIBS@

