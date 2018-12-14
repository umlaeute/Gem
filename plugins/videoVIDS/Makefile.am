AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/videoVIDS.sln win-vs2003/videoVIDS.vcproj
EXTRA_DIST += win-vs2008/videoVIDS.sln win-vs2008/videoVIDS.vcproj

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if WINDOWS
pkglib_LTLIBRARIES+= gem_videoVIDS.la
dist_gemhelp_DATA +=VIDS-videoplugin.pd
endif

gem_videoVIDS_la_CXXFLAGS =
gem_videoVIDS_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoVIDS_la_LDFLAGS += -no-undefined
endif
gem_videoVIDS_la_LIBADD   =

# RTE
gem_videoVIDS_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoVIDS_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoVIDS_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoVIDS_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoVIDS_la @MOREFLAGS@

# Dependencies
gem_videoVIDS_la_CXXFLAGS += -I$(srcdir)/videoInput
gem_videoVIDS_la_LDFLAGS  += -lole32 -loleaut32  -Xlinker -lstrmiids -Xlinker -luuid

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoVIDS_la_SOURCES=         \
	videoInput/videoInput.cpp \
	videoInput/videoInput.h   \
	videoVIDS.cpp             \
	videoVIDS.h
