
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)
AM_CXXFLAGS =

pkglib_LTLIBRARIES=

gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

pkglib_LTLIBRARIES+= gem_recordPNM.la
dist_gemhelp_DATA += PNM-recordplugin.pd

AM_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
AM_LDFLAGS += -no-undefined
endif
gem_recordPNM_la_LIBADD =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_recordPNM_la_LIBADD += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)

# Dependencies
AM_CXXFLAGS +=
gem_recordPNM_la_LIBADD +=

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_recordPNM_la_SOURCES= recordPNM.cpp recordPNM.h
