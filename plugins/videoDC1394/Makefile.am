
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBDC1394_2
pkglib_LTLIBRARIES+= gem_videoDC1394.la
dist_gemhelp_DATA +=dc1394-videoplugin.pd
endif

gem_videoDC1394_la_CXXFLAGS =
gem_videoDC1394_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoDC1394_la_LDFLAGS += -no-undefined
endif
gem_videoDC1394_la_LIBADD   =

# RTE
gem_videoDC1394_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoDC1394_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoDC1394_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoDC1394_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# flags for threading
gem_videoDC1394_la_CXXFLAGS += $(GEM_THREADS_CFLAGS)
gem_videoDC1394_la_LIBADD   += $(GEM_THREADS_LIBS)
# gem_videoDC1394_la @MOREFLAGS@

# Dependencies

gem_videoDC1394_la_CXXFLAGS += $(GEM_LIB_LIBDC1394_2_CFLAGS)
gem_videoDC1394_la_LIBADD   += $(GEM_LIB_LIBDC1394_2_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDC1394_la_SOURCES= videoDC1394.cpp videoDC1394.h

