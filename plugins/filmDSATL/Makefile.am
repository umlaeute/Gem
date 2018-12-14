
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/filmDS.sln win-vs2003/filmDS.vcproj
EXTRA_DIST += win-vs2008/filmDS.sln win-vs2008/filmDS.vcproj

pkglib_LTLIBRARIES=
if WINDOWS
pkglib_LTLIBRARIES+= gem_filmDS.la
endif

gem_filmDS_la_CXXFLAGS =
gem_filmDS_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmDS_la_LDFLAGS += -no-undefined
endif
gem_filmDS_la_LIBADD   =

# RTE
gem_filmDS_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmDS_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmDS_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmDS_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmDS_la @MOREFLAGS@

# Dependencies
# none

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmDS_la_SOURCES= filmDS.cpp filmDS.h

