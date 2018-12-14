
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =

pkglib_LTLIBRARIES=
if HAVE_FRAMEWORK_IMAGEIO
pkglib_LTLIBRARIES += gem_imageIO.la
endif

gem_imageIO_la_OBJCFLAGS = $(GEM_EXTERNAL_CFLAGS) -fobjc-arc
gem_imageIO_la_OBJCXXFLAGS = $(GEM_EXTERNAL_CFLAGS) -fobjc-arc
gem_imageIO_la_LDFLAGS = -module -avoid-version -shared $(GEM_ARCH_LDFLAGS)
gem_imageIO_la_LIBADD =

# RTE
gem_imageIO_la_OBJCFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageIO_la_OBJCXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageIO_la_LDFLAGS += $(GEM_RTE_LIBS) $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageIO_la_OBJCFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageIO_la_OBJCXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageIO_la_LIBADD += -L$(top_builddir)
#$(GEM_EXTERNAL_LIBS)

# Dependencies
gem_imageIO_la_OBJCFLAGS += $(GEM_FRAMEWORK_IMAGEIO_CFLAGS)
gem_imageIO_la_OBJCXXFLAGS += $(GEM_FRAMEWORK_IMAGEIO_CFLAGS)
gem_imageIO_la_LIBADD += $(GEM_FRAMEWORK_IMAGEIO_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk

### SOURCES
gem_imageIO_la_SOURCES=imageIO.h imageIO.mm
