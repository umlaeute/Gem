
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/imageSGI.sln win-vs2003/imageSGI.vcproj
EXTRA_DIST += win-vs2008/imageSGI.sln win-vs2008/imageSGI.vcproj

pkglib_LTLIBRARIES= gem_imageSGI.la

gem_imageSGI_la_CXXFLAGS =
gem_imageSGI_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_imageSGI_la_LDFLAGS += -no-undefined
endif
gem_imageSGI_la_LIBADD   =

# RTE
gem_imageSGI_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageSGI_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageSGI_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageSGI_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_imageSGI_la @MOREFLAGS@

# Dependencies
## none

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageSGI_la_SOURCES= imageSGI.cpp imageSGI.h sgiimage.cpp sgiimage.h

