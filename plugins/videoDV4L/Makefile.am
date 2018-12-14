
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES =
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBRAW1394
if HAVE_LIB_LIBIEC61883
if HAVE_LIB_LIBDV
pkglib_LTLIBRARIES += gem_videoDV4L.la
dist_gemhelp_DATA +=dv4l-videoplugin.pd
endif
endif
endif

gem_videoDV4L_la_CXXFLAGS =
gem_videoDV4L_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoDV4L_la_LDFLAGS += -no-undefined
endif
gem_videoDV4L_la_LIBADD   =

# RTE
gem_videoDV4L_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoDV4L_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoDV4L_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoDV4L_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# flags for threading
gem_videoDV4L_la_CXXFLAGS += $(GEM_THREADS_CFLAGS)
gem_videoDV4L_la_LIBADD   += $(GEM_THREADS_LIBS)
# gem_videoDV4L_la @MOREFLAGS@

# Dependencies
gem_videoDV4L_la_CXXFLAGS += $(GEM_LIB_LIBRAW1394_CFLAGS) $(GEM_LIB_LIBIEC61883_CFLAGS) $(GEM_LIB_LIBDV_CFLAGS)
gem_videoDV4L_la_LIBADD   += $(GEM_LIB_LIBRAW1394_LIBS)   $(GEM_LIB_LIBIEC61883_LIBS)   $(GEM_LIB_LIBDV_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDV4L_la_SOURCES= videoDV4L.cpp videoDV4L.h

