
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)
AM_CXXFLAGS =

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBQUICKTIME
pkglib_LTLIBRARIES+= gem_filmQT4L.la
pkglib_LTLIBRARIES+= gem_recordQT4L.la
dist_gemhelp_DATA += quicktime4linux-recordplugin.pd
else
if HAVE_LIB_LQT
pkglib_LTLIBRARIES+= gem_filmQT4L.la
pkglib_LTLIBRARIES+= gem_recordQT4L.la
dist_gemhelp_DATA += quicktime4linux-recordplugin.pd
endif
endif

AM_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
AM_LDFLAGS += -no-undefined
endif
gem_filmQT4L_la_LIBADD   =
gem_recordQT4L_la_LIBADD =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmQT4L_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
gem_recordQT4L_la_LIBADD += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmQT4L_la @MOREFLAGS@

# Dependencies
AM_CXXFLAGS += $(GEM_LIB_LIBQUICKTIME_CFLAGS) $(GEM_LIB_LQT_CFLAGS)
gem_filmQT4L_la_LIBADD   += $(GEM_LIB_LIBQUICKTIME_LIBS)   $(GEM_LIB_LQT_LIBS)
gem_recordQT4L_la_LIBADD += $(GEM_LIB_LIBQUICKTIME_LIBS)   $(GEM_LIB_LQT_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmQT4L_la_SOURCES  = filmQT4L.cpp filmQT4L.h
gem_recordQT4L_la_SOURCES= recordQT4L.cpp recordQT4L.h

