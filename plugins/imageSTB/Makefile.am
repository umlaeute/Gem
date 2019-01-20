
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/imageSTB.sln win-vs2003/imageSTB.vcproj
EXTRA_DIST += win-vs2008/imageSTB.sln win-vs2008/imageSTB.vcproj

pkglib_LTLIBRARIES= gem_imageSTB.la

gem_imageSTB_la_CXXFLAGS =
gem_imageSTB_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_imageSTB_la_LDFLAGS += -no-undefined
endif
gem_imageSTB_la_LIBADD   =

if HAVE_LIB_STB
else
AM_CPPFLAGS += -I$(srcdir)/STB
endif

# RTE
gem_imageSTB_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_imageSTB_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_imageSTB_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_imageSTB_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_imageSTB_la @MOREFLAGS@

# Dependencies
gem_imageSTB_la_CXXFLAGS += $(GEM_LIB_STB_CFLAGS)
gem_imageSTB_la_LIBADD   += $(GEM_LIB_STB_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageSTB_la_SOURCES= imageSTB.cpp imageSTB.h

