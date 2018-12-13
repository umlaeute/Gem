
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
if HAVE_LIB_GMERLIN_AVDEC
pkglib_LTLIBRARIES+= gem_filmGMERLIN.la
endif

gem_filmGMERLIN_la_CXXFLAGS =
gem_filmGMERLIN_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmGMERLIN_la_LDFLAGS += -no-undefined
endif
gem_filmGMERLIN_la_LIBADD   =

# RTE
gem_filmGMERLIN_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmGMERLIN_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmGMERLIN_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmGMERLIN_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmGMERLIN_la @MOREFLAGS@

# Dependencies
gem_filmGMERLIN_la_CXXFLAGS += $(GEM_LIB_GMERLIN_AVDEC_CFLAGS)
gem_filmGMERLIN_la_LIBADD   += $(GEM_LIB_GMERLIN_AVDEC_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmGMERLIN_la_SOURCES= filmGMERLIN.cpp filmGMERLIN.h

