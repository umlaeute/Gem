
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

EXTRA_DIST =
EXTRA_DIST += win-vs2003/modelOBJ.sln win-vs2003/modelOBJ.vcproj
EXTRA_DIST += win-vs2008/modelOBJ.sln win-vs2008/modelOBJ.vcproj

pkglib_LTLIBRARIES= gem_modelOBJ.la

gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =
dist_gemhelp_DATA += OBJ-modelplugin.pd

gem_modelOBJ_la_CXXFLAGS =
gem_modelOBJ_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_modelOBJ_la_LDFLAGS += -no-undefined
endif
gem_modelOBJ_la_LIBADD   =

# RTE
gem_modelOBJ_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
gem_modelOBJ_la_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gem_modelOBJ_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_modelOBJ_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gem_modelOBJ_la @MOREFLAGS@

# Dependencies
gem_modelOBJ_la_CXXFLAGS += @GL_CFLAGS@
gem_modelOBJ_la_LIBADD   += @GL_LIBS@ @GL_LDFLAGS@



# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_modelOBJ_la_SOURCES= \
	modelOBJ.cpp \
	modelOBJ.h \
	model_loader.cpp \
	model_loader.h

