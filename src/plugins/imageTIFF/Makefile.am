
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_imageTIFF.la
gem_imageTIFF_la_CXXFLAGS =
gem_imageTIFF_la_LDFLAGS  = -module -avoid-version -shared
gem_imageTIFF_la_LIBADD   =

# RTE
gem_imageTIFF_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_imageTIFF_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_imageTIFF_la_CXXFLAGS += @GEM_LIB_TIFF_CFLAGS@
gem_imageTIFF_la_LIBADD   += @GEM_LIB_TIFF_LIBS@

gem_imageTIFF_la_SOURCES= imageTIFF.cpp imageTIFF.h

