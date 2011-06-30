
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I$(srcdir)/../..

pkglib_LTLIBRARIES= gem_imageQT.la

gem_imageQT_la_CXXFLAGS =
gem_imageQT_la_LDFLAGS  = -module -avoid-version -shared
gem_imageQT_la_LIBADD   =

# RTE
gem_imageQT_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
gem_imageQT_la_LDFLAGS  += @GEM_RTE_LIBS@   @GEM_ARCH_LDFLAGS@

# Dependencies
gem_imageQT_la_CXXFLAGS += @GEM_FRAMEWORK_QUICKTIME@
gem_imageQT_la_LIBADD   += @GEM_FRAMEWORK_QUICKTIME@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_imageQT_la_SOURCES= imageQT.cpp imageQT.h

