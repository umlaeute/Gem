
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/imageMAGICK.sln win-vs2003/imageMAGICK.vcproj
EXTRA_DIST += win-vs2008/imageMAGICK.sln win-vs2008/imageMAGICK.vcproj
EXTRA_DIST += win-vs2008/ImageMagick_Release.vsprops win-vs2008/ImageMagick.vsprops

pkglib_LTLIBRARIES=
if HAVE_LIB_IMAGEMAGICK__
pkglib_LTLIBRARIES+= gem_imageMAGICK.la
else
if HAVE_LIB_MAGICKCORE
pkglib_LTLIBRARIES+= gem_imageMAGICK.la
endif
endif

gem_imageMAGICK_la_CXXFLAGS =
gem_imageMAGICK_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_imageMAGICK_la_LDFLAGS += -no-undefined
endif
gem_imageMAGICK_la_LIBADD   =

# RTE
gem_imageMAGICK_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageMAGICK_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageMAGICK_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageMAGICK_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_imageMAGICK_la @MOREFLAGS@

# Dependencies
if HAVE_LIB_IMAGEMAGICK__
gem_imageMAGICK_la_CXXFLAGS += $(GEM_LIB_IMAGEMAGICK___CFLAGS)
gem_imageMAGICK_la_LIBADD   += $(GEM_LIB_IMAGEMAGICK___LIBS)
else
if HAVE_LIB_MAGICKCORE
gem_imageMAGICK_la_CXXFLAGS += $(GEM_LIB_MAGICKCORE_CFLAGS)
gem_imageMAGICK_la_LIBADD   += $(GEM_LIB_MAGICKCORE_LIBS)
endif
endif


# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageMAGICK_la_SOURCES = imageMAGICK.cpp imageMAGICK.h

if HAVE_LIB_IMAGEMAGICK__
gem_imageMAGICK_la_SOURCES+= MagickPlusPlus.cpp
else
if HAVE_LIB_MAGICKCORE
gem_imageMAGICK_la_SOURCES+= MagickCore.cpp
endif
endif
