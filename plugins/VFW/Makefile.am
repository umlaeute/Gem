ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST+=win-vs2003/videoVFW.sln win-vs2003/videoVFW.vcproj
EXTRA_DIST+=win-vs2008/videoVFW.sln win-vs2008/videoVFW.vcproj

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if WINDOWS
pkglib_LTLIBRARIES+= gem_videoVFW.la
dist_gemhelp_DATA +=VFW-videoplugin.pd
endif

gem_videoVFW_la_CXXFLAGS =
gem_videoVFW_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoVFW_la_LDFLAGS += -no-undefined
endif
gem_videoVFW_la_LIBADD   =

# RTE
gem_videoVFW_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoVFW_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoVFW_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoVFW_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoVFW_la @MOREFLAGS@

# Dependencies
gem_videoVFW_la_CXXFLAGS += $(GEM_LIB_VFW32_CFLAGS)
gem_videoVFW_la_LIBADD   += $(GEM_LIB_VFW32_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoVFW_la_SOURCES= videoVFW.cpp videoVFW.h

