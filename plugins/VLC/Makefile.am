
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBVLC
pkglib_LTLIBRARIES+= gem_videoVLC.la
dist_gemhelp_DATA +=vlc-videoplugin.pd
endif

gem_videoVLC_la_CXXFLAGS =
gem_videoVLC_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoVLC_la_LDFLAGS += -no-undefined
endif
gem_videoVLC_la_LIBADD   =

# RTE
gem_videoVLC_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoVLC_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoVLC_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoVLC_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoVLC_la @MOREFLAGS@

# Dependencies

gem_videoVLC_la_CXXFLAGS += $(GEM_LIB_LIBVLC_CFLAGS)
gem_videoVLC_la_LIBADD   += $(GEM_LIB_LIBVLC_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoVLC_la_SOURCES= videoVLC.cpp videoVLC.h

