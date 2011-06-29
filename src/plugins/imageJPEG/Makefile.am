
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_imageJPEG.la
gem_imageJPEG_la_CXXFLAGS =
gem_imageJPEG_la_LDFLAGS  = -module -avoid-version -shared
gem_imageJPEG_la_LIBADD   =

# RTE
gem_imageJPEG_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_imageJPEG_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_imageJPEG_la_CXXFLAGS += @GEM_LIB_JPEG_CFLAGS@
gem_imageJPEG_la_LIBADD   += @GEM_LIB_JPEG_LIBS@

### SOURCES
gem_imageJPEG_la_SOURCES= imageJPEG.cpp imageJPEG.h

