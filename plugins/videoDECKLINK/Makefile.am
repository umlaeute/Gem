ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../../src

EXTRA_DIST = LICENSE.txt README.txt

plugindir=$(libdir)/Gem

plugin_LTLIBRARIES=
dist_plugin_DATA  =

if HAVE_LIB_DECKLINK
plugin_LTLIBRARIES+= gem_videoDECKLINK.la
dist_plugin_DATA  += decklink-videoplugin.pd
endif

gem_videoDECKLINK_la_CXXFLAGS =
gem_videoDECKLINK_la_LDFLAGS  = -module -avoid-version -shared
gem_videoDECKLINK_la_LIBADD   =

if WINDOWS
gem_videoDECKLINK_la_LDFLAGS += -no-undefined
endif

# RTE
gem_videoDECKLINK_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoDECKLINK_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoDECKLINK_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS) $(GEM_THREADS_CFLAGS)
gem_videoDECKLINK_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS) $(GEM_THREADS_LIBS)
# #gem_videoDECKLINK_la @MOREFLAGS@

# Dependencies
gem_videoDECKLINK_la_CXXFLAGS += $(GEM_LIB_DECKLINK_CFLAGS)
gem_videoDECKLINK_la_LIBADD   += $(GEM_LIB_DECKLINK_LIBS)

if WINDOWS
gem_videoDECKLINK_la_LIBADD   += -lole32
endif

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDECKLINK_la_SOURCES= videoDECKLINK.cpp videoDECKLINK.h

