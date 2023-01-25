
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=
if HAVE_FFMPEG
pkglib_LTLIBRARIES+= gem_filmFFMPEG.la
endif

gem_filmFFMPEG_la_CXXFLAGS =
gem_filmFFMPEG_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_filmFFMPEG_la_LDFLAGS += -no-undefined
endif
gem_filmFFMPEG_la_LIBADD   =

# RTE
gem_filmFFMPEG_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_filmFFMPEG_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_filmFFMPEG_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_filmFFMPEG_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_filmFFMPEG_la @MOREFLAGS@

# Dependencies
gem_filmFFMPEG_la_CXXFLAGS += $(GEM_LIB_FFMPEG_CFLAGS)
gem_filmFFMPEG_la_LIBADD   += $(GEM_LIB_FFMPEG_LIBS)

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_filmFFMPEG_la_SOURCES= filmFFMPEG.cpp filmFFMPEG.h

