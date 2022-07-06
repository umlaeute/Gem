
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES=

if HAVE_ASSIMP3
pkglib_LTLIBRARIES += gem_modelASSIMP3.la
endif

gem_modelASSIMP3_la_CXXFLAGS =
gem_modelASSIMP3_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_modelASSIMP3_la_LDFLAGS += -no-undefined
endif
gem_modelASSIMP3_la_LIBADD   =

# RTE
gem_modelASSIMP3_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_modelASSIMP3_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_modelASSIMP3_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_modelASSIMP3_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_modelASSIMP3_la @MOREFLAGS@

# Dependencies
gem_modelASSIMP3_la_CXXFLAGS += @PKG_ASSIMP_CFLAGS@ @GL_CFLAGS@
gem_modelASSIMP3_la_LIBADD   += @PKG_ASSIMP_LIBS@ @GL_LIBS@ @GL_LDFLAGS@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_modelASSIMP3_la_SOURCES= \
	modelASSIMP3.cpp \
	modelASSIMP3.h
