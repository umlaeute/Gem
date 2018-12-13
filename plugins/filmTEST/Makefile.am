
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES= gem_filmTEST.la

gem_filmTEST_la_CXXFLAGS =
gem_filmTEST_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmTEST_la_LDFLAGS += -no-undefined
endif
gem_filmTEST_la_LIBADD   =

# RTE
gem_filmTEST_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmTEST_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmTEST_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmTEST_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmTEST_la @MOREFLAGS@

# Dependencies
gem_filmTEST_la_CXXFLAGS +=
gem_filmTEST_la_LIBADD   +=

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmTEST_la_SOURCES= filmTEST.cpp filmTEST.h

