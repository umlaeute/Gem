
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if MACOSX
if HAVE_FRAMEWORK_QUICKTIME
if HAVE_FRAMEWORK_CARBON
pkglib_LTLIBRARIES+=gem_videoDarwin.la
dist_gemhelp_DATA +=Darwin-videoplugin.pd
endif
endif
endif

gem_videoDarwin_la_CXXFLAGS =
gem_videoDarwin_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoDarwin_la_LDFLAGS += -no-undefined
endif
gem_videoDarwin_la_LIBADD   =

# RTE
gem_videoDarwin_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoDarwin_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoDarwin_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoDarwin_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoDarwin_la @MOREFLAGS@

# Dependencies
gem_videoDarwin_la_CXXFLAGS += $(GEM_FRAMEWORK_QUICKTIME_CFLAGS) $(GEM_FRAMEWORK_CARBON_CFLAGS)
gem_videoDarwin_la_LIBADD   += $(GEM_FRAMEWORK_QUICKTIME_LIBS) $(GEM_FRAMEWORK_CARBON_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDarwin_la_SOURCES= videoDarwin.cpp videoDarwin.h


