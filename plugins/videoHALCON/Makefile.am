
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../../src

EXTRA_DIST = halcon.m4 LICENSE.txt README.txt
EXTRA_DIST += win-vs2003/videoHALCON.sln win-vs2003/videoHALCON.vcproj
EXTRA_DIST += win-vs2008/videoHALCON.sln win-vs2008/videoHALCON.vcproj
EXTRA_DIST += win-vs2008/HALCON.vsprops

plugindir=$(libdir)/Gem

plugin_LTLIBRARIES=
dist_plugin_DATA  =

if HAVE_HALCON
plugin_LTLIBRARIES+= gem_videoHALCON.la
dist_plugin_DATA  += halcon-videoplugin.pd
endif

gem_videoHALCON_la_CXXFLAGS =
gem_videoHALCON_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoHALCON_la_LDFLAGS += -no-undefined
endif
gem_videoHALCON_la_LIBADD   =

# RTE
gem_videoHALCON_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoHALCON_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoHALCON_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoHALCON_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
# #gem_videoHALCON_la @MOREFLAGS@

# Dependencies
gem_videoHALCON_la_CXXFLAGS += $(GEM_HALCON_CXXFLAGS)
gem_videoHALCON_la_LIBADD   += $(GEM_HALCON_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoHALCON_la_SOURCES= videoHALCON.cpp videoHALCON.h

