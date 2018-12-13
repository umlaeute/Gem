
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../../src

EXTRA_DIST = avt.m4 LICENSE.txt README.txt
EXTRA_DIST += win-vs2003/videoAVT.sln win-vs2003/videoAVT.vcproj
EXTRA_DIST += win-vs2008/videoAVT.sln win-vs2008/videoAVT.vcproj

plugindir=$(libdir)/Gem

plugin_LTLIBRARIES=
dist_plugin_DATA  =

if HAVE_LIB_LIBPVAPI
plugin_LTLIBRARIES+= gem_videoAVT.la
dist_plugin_DATA  += avt-videoplugin.pd
endif

gem_videoAVT_la_CXXFLAGS =
gem_videoAVT_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoAVT_la_LDFLAGS += -no-undefined
endif
gem_videoAVT_la_LIBADD   =

# RTE
gem_videoAVT_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoAVT_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoAVT_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoAVT_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
# #gem_videoAVT_la @MOREFLAGS@

# Dependencies
gem_videoAVT_la_CXXFLAGS += $(GEM_LIB_LIBPVAPI_CFLAGS)
gem_videoAVT_la_LIBADD   += $(GEM_LIB_LIBPVAPI_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoAVT_la_SOURCES= videoAVT.cpp videoAVT.h

