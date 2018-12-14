
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/imageTIFF.sln win-vs2003/imageTIFF.vcproj
EXTRA_DIST += win-vs2008/imageTIFF.sln win-vs2008/imageTIFF.vcproj
EXTRA_DIST += win-vs2008/TIFF.vsprops

pkglib_LTLIBRARIES=
if HAVE_LIB_TIFF
pkglib_LTLIBRARIES+= gem_imageTIFF.la
endif


gem_imageTIFF_la_CXXFLAGS =
gem_imageTIFF_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_imageTIFF_la_LDFLAGS += -no-undefined
endif
gem_imageTIFF_la_LIBADD   =

# RTE
gem_imageTIFF_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageTIFF_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageTIFF_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageTIFF_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_imageTIFF_la @MOREFLAGS@

# Dependencies
gem_imageTIFF_la_CXXFLAGS += $(GEM_LIB_TIFF_CFLAGS)
gem_imageTIFF_la_LIBADD   += $(GEM_LIB_TIFF_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageTIFF_la_SOURCES= imageTIFF.cpp imageTIFF.h

