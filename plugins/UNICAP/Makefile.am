
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

if HAVE_LIB_LIBUNICAP
pkglib_LTLIBRARIES+= gem_videoUNICAP.la
dist_gemhelp_DATA +=unicap-videoplugin.pd
endif

gem_videoUNICAP_la_CXXFLAGS =
gem_videoUNICAP_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoUNICAP_la_LDFLAGS += -no-undefined
endif
gem_videoUNICAP_la_LIBADD   =

# RTE
gem_videoUNICAP_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_videoUNICAP_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_videoUNICAP_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoUNICAP_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoUNICAP_la @MOREFLAGS@

# Dependencies
gem_videoUNICAP_la_CXXFLAGS += $(GEM_LIB_LIBUNICAP_CFLAGS)
gem_videoUNICAP_la_LIBADD   += $(GEM_LIB_LIBUNICAP_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoUNICAP_la_SOURCES= videoUNICAP.cpp videoUNICAP.h

