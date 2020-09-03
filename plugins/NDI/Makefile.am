
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../../src

EXTRA_DIST = ndi.m4 LICENSE.txt README.txt

plugindir=$(libdir)/Gem

plugin_LTLIBRARIES=
dist_plugin_DATA  =

if HAVE_NDI
plugin_LTLIBRARIES+= gem_videoNDI.la gem_recordNDI.la
dist_plugin_DATA  += ndi-videoplugin.pd
endif

# videoNDI

AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
AM_LDFLAGS += -no-undefined
endif
if MACOSX
else
AM_LDFLAGS  += -Wl,-rpath='$$ORIGIN'
endif

gem_videoNDI_la_LIBADD   =
gem_recordNDI_la_LIBADD   =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoNDI_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
gem_recordNDI_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
# #gem_videoNDI_la @MOREFLAGS@

# Dependencies
AM_CXXFLAGS += $(GEM_NDI_CXXFLAGS)
gem_videoNDI_la_LIBADD   += $(GEM_NDI_LIBS)
gem_recordNDI_la_LIBADD   += $(GEM_NDI_LIBS)


# convenience symlinks
-include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoNDI_la_SOURCES= videoNDI.cpp videoNDI.h init_ndi_library.hh
gem_recordNDI_la_SOURCES= recordNDI.cpp recordNDI.h init_ndi_library.hh

