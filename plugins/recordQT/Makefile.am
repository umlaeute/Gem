ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/recordQT.sln win-vs2003/recordQT.vcproj
EXTRA_DIST += win-vs2008/recordQT.sln win-vs2008/recordQT.vcproj

pkglib_LTLIBRARIES=
if HAVE_FRAMEWORK_QUICKTIME
pkglib_LTLIBRARIES+= gem_recordQT.la
endif

gem_recordQT_la_CXXFLAGS =
gem_recordQT_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_recordQT_la_LDFLAGS += -no-undefined
endif
gem_recordQT_la_LIBADD   =

# RTE
gem_recordQT_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_recordQT_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_recordQT_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_recordQT_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_recordQT_la @MOREFLAGS@

# Dependencies
gem_recordQT_la_CXXFLAGS += $(GEM_FRAMEWORK_QUICKTIME_CFLAGS)
gem_recordQT_la_LIBADD   += $(GEM_FRAMEWORK_QUICKTIME_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_recordQT_la_SOURCES= recordQT.cpp recordQT.h

