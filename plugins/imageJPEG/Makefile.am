
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/imageJPEG.sln win-vs2003/imageJPEG.vcproj
EXTRA_DIST += win-vs2008/imageJPEG.sln win-vs2008/imageJPEG.vcproj
EXTRA_DIST += win-vs2008/JPEG.vsprops

pkglib_LTLIBRARIES=
if HAVE_LIB_JPEG
pkglib_LTLIBRARIES+= gem_imageJPEG.la
endif

gem_imageJPEG_la_CXXFLAGS =
gem_imageJPEG_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_imageJPEG_la_LDFLAGS += -no-undefined
endif
gem_imageJPEG_la_LIBADD   =

# RTE
gem_imageJPEG_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageJPEG_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageJPEG_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageJPEG_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_imageJPEG_la @MOREFLAGS@

# Dependencies
gem_imageJPEG_la_CXXFLAGS += $(GEM_LIB_JPEG_CFLAGS)
gem_imageJPEG_la_LIBADD   += $(GEM_LIB_JPEG_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageJPEG_la_SOURCES= imageJPEG.cpp imageJPEG.h

