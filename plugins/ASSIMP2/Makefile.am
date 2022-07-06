
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/modelASSIMP2.sln win-vs2003/modelASSIMP2.vcproj
EXTRA_DIST += win-vs2008/modelASSIMP2.sln win-vs2008/modelASSIMP2.vcproj
EXTRA_DIST += win-vs2008/ASSIMP2.vsprops

pkglib_LTLIBRARIES=

if HAVE_ASSIMP2
pkglib_LTLIBRARIES += gem_modelASSIMP2.la
endif

gem_modelASSIMP2_la_CXXFLAGS =
gem_modelASSIMP2_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_modelASSIMP2_la_LDFLAGS += -no-undefined
endif
gem_modelASSIMP2_la_LIBADD   =

# RTE
gem_modelASSIMP2_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_modelASSIMP2_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_modelASSIMP2_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_modelASSIMP2_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_modelASSIMP2_la @MOREFLAGS@

# Dependencies
gem_modelASSIMP2_la_CXXFLAGS += @PKG_ASSIMP_CFLAGS@ @GL_CFLAGS@
gem_modelASSIMP2_la_LIBADD   += @PKG_ASSIMP_LIBS@ @GL_LIBS@ @GL_LDFLAGS@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_modelASSIMP2_la_SOURCES= \
	modelASSIMP2.cpp \
	modelASSIMP2.h

