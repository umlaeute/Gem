
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =

pkglib_LTLIBRARIES=
if HAVE_FRAMEWORK_AVFOUNDATION
pkglib_LTLIBRARIES += gem_videoAVF.la
endif

gem_videoAVF_la_OBJCFLAGS = $(GEM_EXTERNAL_CFLAGS) -fobjc-arc
gem_videoAVF_la_OBJCXXFLAGS = $(GEM_EXTERNAL_CFLAGS) -fobjc-arc
gem_videoAVF_la_LDFLAGS = -module -avoid-version -shared $(GEM_ARCH_LDFLAGS)
gem_videoAVF_la_LIBADD =

# RTE
gem_videoAVF_la_OBJCFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoAVF_la_OBJCXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoAVF_la_LDFLAGS += $(GEM_RTE_LIBS) $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoAVF_la_OBJCFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoAVF_la_OBJCXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoAVF_la_LIBADD += -L$(top_builddir)
#$(GEM_EXTERNAL_LIBS)

# Dependencies
gem_videoAVF_la_OBJCFLAGS += $(GEM_FRAMEWORK_AVFOUNDATION_CFLAGS)
gem_videoAVF_la_OBJCXXFLAGS += $(GEM_FRAMEWORK_AVFOUNDATION_CFLAGS)
gem_videoAVF_la_LIBADD += $(GEM_FRAMEWORK_AVFOUNDATION_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk

### SOURCES
gem_videoAVF_la_SOURCES= AVFVideoGrabber.h AVFVideoGrabber.mm videoAVF.h videoAVF.mm
