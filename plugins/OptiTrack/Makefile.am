ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += README.txt
EXTRA_DIST += win-vs2003/videoOptiTrack.sln win-vs2003/videoOptiTrack.vcproj
EXTRA_DIST += win-vs2008/videoOptiTrack.sln win-vs2008/videoOptiTrack.vcproj
EXTRA_DIST += win-vs2008/OptiTrack.vsprops

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if DISABLED
pkglib_LTLIBRARIES+= gem_videoOptiTrack.la
dist_gemhelp_DATA +=OptiTrack-videoplugin.pd
endif

gem_videoOptiTrack_la_CXXFLAGS =
gem_videoOptiTrack_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoOptiTrack_la_LDFLAGS += -no-undefined
endif
gem_videoOptiTrack_la_LIBADD   =

# RTE
gem_videoOptiTrack_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoOptiTrack_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoOptiTrack_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoOptiTrack_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoOptiTrack_la @MOREFLAGS@

# Dependencies
#gem_videoOptiTrack_la_CXXFLAGS += $(GEM_LIB_LIBOPTITRACK_CFLAGS)
#gem_videoOptiTrack_la_LIBADD   += $(GEM_LIB_LIBOPTITRACK_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoOptiTrack_la_SOURCES= videoOptiTrack.cpp videoOptiTrack.h

