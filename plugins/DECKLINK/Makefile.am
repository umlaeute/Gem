ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES =
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

EXTRA_DIST = LICENSE.txt README.txt

if HAVE_LIB_DECKLINK
pkglib_LTLIBRARIES+= gem_videoDECKLINK.la
dist_gemhelp_DATA  += decklink-videoplugin.pd
else
if GEM_DECKLINK_LOCAL
pkglib_LTLIBRARIES+= gem_videoDECKLINK.la
dist_gemhelp_DATA  += decklink-videoplugin.pd
endif
endif

AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared
gem_videoDECKLINK_la_LIBADD   =
if WINDOWS
AM_LDFLAGS += -no-undefined
endif
if MACOSX
# nothing todo on macOS
else
AM_LDFLAGS  += -Wl,-rpath='$$ORIGIN'
endif

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS) $(GEM_THREADS_CFLAGS)
gem_videoDECKLINK_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS) $(GEM_THREADS_LIBS)
# #gem_videoDECKLINK_la @MOREFLAGS@

# Dependencies
if GEM_DECKLINK_LOCAL
AM_CXXFLAGS += -I$(srcdir)/SDK/$(GEM_DECKLINK_SYSTEM)
endif
AM_CXXFLAGS += $(GEM_LIB_DECKLINK_CFLAGS)
gem_videoDECKLINK_la_LIBADD   += $(GEM_LIB_DECKLINK_LIBS)

if WINDOWS
gem_videoDECKLINK_la_LIBADD   += -lole32
endif

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDECKLINK_la_SOURCES= videoDECKLINK.cpp videoDECKLINK.h
