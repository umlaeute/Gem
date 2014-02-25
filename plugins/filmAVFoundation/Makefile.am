
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src @GEM_EXTERNAL_CPPFLAGS@

EXTRA_DIST =

pkglib_LTLIBRARIES= 
if HAVE_FRAMEWORK_AVFOUNDATION
pkglib_LTLIBRARIES += gem_filmAVFoundation.la
endif

gem_filmAVFoundation_la_OBJCXXFLAGS =
gem_filmAVFoundation_la_LDFLAGS  = -module -avoid-version -shared
gem_filmAVFoundation_la_LIBADD   =

# RTE
gem_filmAVFoundation_la_OBJCXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_filmAVFoundation_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@
# flags for building Gem externals
gem_filmAVFoundation_la_OBJCXXFLAGS += @GEM_EXTERNAL_CFLAGS@
gem_filmAVFoundation_la_LIBADD   += -L$(top_builddir)
#@GEM_EXTERNAL_LIBS@

# Dependencies
gem_filmAVFoundation_la_OBJCXXFLAGS += @GEM_FRAMEWORK_AVFOUNDATION@
#gem_filmAVFoundation_la_LIBADD   += @GEM_FRAMEWORK_AVFOUNDATION@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk

### SOURCES
gem_filmAVFoundation_la_SOURCES= AVFMoviePlayer.h AVFMoviePlayer.m filmAVFoundation.mm filmAVFoundation.h
