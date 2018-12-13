
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
if HAVE_LIB_LIBMPEG3
pkglib_LTLIBRARIES+= gem_filmMPEG3.la
endif


gem_filmMPEG3_la_CXXFLAGS =
gem_filmMPEG3_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmMPEG3_la_LDFLAGS += -no-undefined
endif
gem_filmMPEG3_la_LIBADD   =

# RTE
gem_filmMPEG3_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmMPEG3_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmMPEG3_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmMPEG3_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmMPEG3_la @MOREFLAGS@

# Dependencies
gem_filmMPEG3_la_CXXFLAGS += $(GEM_LIB_LIBMPEG3_CFLAGS)
gem_filmMPEG3_la_LIBADD   += $(GEM_LIB_LIBMPEG3_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmMPEG3_la_SOURCES= filmMPEG3.cpp filmMPEG3.h
