
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
if HAVE_LIB_AVIFILE
pkglib_LTLIBRARIES+= gem_filmAVIPLAY.la
endif

gem_filmAVIPLAY_la_CXXFLAGS =
gem_filmAVIPLAY_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmAVIPLAY_la_LDFLAGS += -no-undefined
endif
gem_filmAVIPLAY_la_LIBADD   =

# RTE
gem_filmAVIPLAY_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmAVIPLAY_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmAVIPLAY_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmAVIPLAY_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmAVIPLAY_la @MOREFLAGS@

# Dependencies
gem_filmAVIPLAY_la_CXXFLAGS += $(GEM_LIB_AVIFILE_CFLAGS)
gem_filmAVIPLAY_la_LIBADD   += $(GEM_LIB_AVIFILE_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmAVIPLAY_la_SOURCES= filmAVIPLAY.cpp filmAVIPLAY.h
