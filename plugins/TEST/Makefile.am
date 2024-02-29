
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)
AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared


gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =
pkglib_LTLIBRARIES =

pkglib_LTLIBRARIES+=gem_filmTEST.la
dist_gemhelp_DATA +=test-filmplugin.pd
pkglib_LTLIBRARIES+=gem_videoTEST.la
dist_gemhelp_DATA +=test-videoplugin.pd

if WINDOWS
AM_LDFLAGS += -no-undefined
endif
gem_filmTEST_la_LIBADD   =
gem_videoTEST_la_LIBADD  =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmTEST_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
gem_videoTEST_la_LIBADD  += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoTEST_la @MOREFLAGS@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmTEST_la_SOURCES = filmTEST.cpp filmTEST.h
gem_videoTEST_la_SOURCES= videoTEST.cpp videoTEST.h

