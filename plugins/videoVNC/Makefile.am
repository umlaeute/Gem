
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBVNCCLIENT
pkglib_LTLIBRARIES+=gem_videoVNC.la
dist_gemhelp_DATA +=vnc-videoplugin.pd
endif

gem_videoVNC_la_CXXFLAGS =
gem_videoVNC_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoVNC_la_LDFLAGS += -no-undefined
endif
gem_videoVNC_la_LIBADD   =

# RTE
gem_videoVNC_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoVNC_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoVNC_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoVNC_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoVNC_la @MOREFLAGS@

# Dependencies

gem_videoVNC_la_CXXFLAGS += $(GEM_LIB_LIBVNCCLIENT_CFLAGS)
gem_videoVNC_la_LIBADD   += $(GEM_LIB_LIBVNCCLIENT_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoVNC_la_SOURCES= videoVNC.cpp videoVNC.h

