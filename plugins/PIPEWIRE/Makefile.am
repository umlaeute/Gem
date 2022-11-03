
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)
AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared


gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =


pkglib_LTLIBRARIES =

if HAVE_PIPEWIRE
pkglib_LTLIBRARIES+=gem_videoPIPEWIRE.la
endif

if WINDOWS
AM_LDFLAGS += -no-undefined
endif
gem_videoPIPEWIRE_la_LIBADD  =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoPIPEWIRE_la_LIBADD  += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_videoPIPEWIRE_la @MOREFLAGS@

# Dependencies
AM_CXXFLAGS += $(GEM_LIB_PIPEWIRE_CFLAGS)
gem_videoPIPEWIRE_la_LIBADD   += $(GEM_LIB_PIPEWIRE_LIBS)


# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoPIPEWIRE_la_SOURCES= videoPIPEWIRE.cpp videoPIPEWIRE.h

