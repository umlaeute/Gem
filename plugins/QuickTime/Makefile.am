
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)
AM_CXXFLAGS =

EXTRA_DIST =
EXTRA_DIST += win-vs2003/filmQT.sln win-vs2003/filmQT.vcproj
EXTRA_DIST += win-vs2008/filmQT.sln win-vs2008/filmQT.vcproj

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_FRAMEWORK_QUICKTIME
pkglib_LTLIBRARIES+= gem_filmQT.la
pkglib_LTLIBRARIES+= gem_imageQT.la
dist_gemhelp_DATA += QuickTime-imageloaderplugin.pd QuickTime-imagesaverplugin.pd
dist_gemhelp_DATA += QuickTime-filmplugin.pd QuickTime-recordplugin.pd
pkglib_LTLIBRARIES+= gem_recordQT.la
endif

AM_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
AM_LDFLAGS += -no-undefined
endif

AM_LIBADD =
gem_filmQT_la_LIBADD   =
gem_imageQT_la_LIBADD  =
gem_recordQT_la_LIBADD =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
AM_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmQT_la @MOREFLAGS@

# Dependencies
AM_CXXFLAGS += $(GEM_FRAMEWORK_QUICKTIME_CFLAGS)
AM_LIBADD   += $(GEM_FRAMEWORK_QUICKTIME_LIBS)


gem_filmQT_la_LIBADD   += $(AM_LIBADD)
gem_imageQT_la_LIBADD  += $(AM_LIBADD)
gem_recordQT_la_LIBADD += $(AM_LIBADD)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmQT_la_SOURCES  = filmQT.cpp filmQT.h
gem_imageQT_la_SOURCES = imageQT.cpp imageQT.h
gem_recordQT_la_SOURCES= recordQT.cpp recordQT.h
