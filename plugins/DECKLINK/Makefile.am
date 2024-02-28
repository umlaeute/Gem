ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES =
noinst_LTLIBRARIES =

gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

EXTRA_DIST =
EXTRA_DIST += LICENSE.txt README.txt
EXTRA_DIST += \
	SDK/Windows/DeckLinkAPI_i.c \
	SDK/Windows/DeckLinkAPIDispatch.cpp \
	SDK/Windows/DeckLinkAPI.h \
	SDK/Linux/LinuxCOM.h \
	SDK/Linux/DeckLinkAPIVersion.h \
	SDK/Linux/DeckLinkAPITypes.h \
	SDK/Linux/DeckLinkAPIModes.h \
	SDK/Linux/DeckLinkAPIDispatch.cpp \
	SDK/Linux/DeckLinkAPIDiscovery.h \
	SDK/Linux/DeckLinkAPIDeckControl.h \
	SDK/Linux/DeckLinkAPIConfiguration.h \
	SDK/Linux/DeckLinkAPI.h \
	SDK/Darwin/DeckLinkAPIVersion.h \
	SDK/Darwin/DeckLinkAPITypes.h \
	SDK/Darwin/DeckLinkAPIStreaming.h \
	SDK/Darwin/DeckLinkAPIModes.h \
	SDK/Darwin/DeckLinkAPIDispatch.cpp \
	SDK/Darwin/DeckLinkAPIDiscovery.h \
	SDK/Darwin/DeckLinkAPIDeckControl.h \
	SDK/Darwin/DeckLinkAPIConfiguration.h \
	SDK/Darwin/DeckLinkAPI.h \
	$(empty)

if HAVE_LIB_DECKLINK
pkglib_LTLIBRARIES+= gem_videoDECKLINK.la gem_recordDECKLINK.la
dist_gemhelp_DATA += decklink-videoplugin.pd
dist_gemhelp_DATA += decklink-recordplugin.pd
else
if GEM_DECKLINK_LOCAL
pkglib_LTLIBRARIES+= gem_videoDECKLINK.la gem_recordDECKLINK.la
dist_gemhelp_DATA += decklink-videoplugin.pd
dist_gemhelp_DATA += decklink-recordplugin.pd
endif
endif

AM_CFLAGS   =
AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared
gem_videoDECKLINK_la_LIBADD      =
gem_recordDECKLINK_la_LIBADD     =
libDeckLinkAPIDispatch_la_LIBADD =

if WINDOWS
AM_LDFLAGS += -no-undefined
endif
if MACOSX
# nothing todo on macOS
else
AM_LDFLAGS  += -Wl,-rpath='$$ORIGIN'
endif

# RTE
AM_CFLAGS   += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CFLAGS   += $(GEM_EXTERNAL_CFLAGS) $(GEM_THREADS_CFLAGS)
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS) $(GEM_THREADS_CFLAGS)

gem_videoDECKLINK_la_LIBADD      += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS) $(GEM_THREADS_LIBS)
gem_recordDECKLINK_la_LIBADD     += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS) $(GEM_THREADS_LIBS)
libDeckLinkAPIDispatch_la_LIBADD += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS) $(GEM_THREADS_LIBS)
# #gem_videoDECKLINK_la @MOREFLAGS@

# Dependencies
if GEM_DECKLINK_LOCAL
AM_CFLAGS   += -I$(srcdir)/SDK/$(GEM_DECKLINK_SYSTEM)
AM_CXXFLAGS += -I$(srcdir)/SDK/$(GEM_DECKLINK_SYSTEM)
endif
AM_CFLAGS   += $(GEM_LIB_DECKLINK_CFLAGS)
AM_CXXFLAGS += $(GEM_LIB_DECKLINK_CFLAGS)
gem_videoDECKLINK_la_LIBADD  += $(GEM_LIB_DECKLINK_LIBS)
gem_recordDECKLINK_la_LIBADD += $(GEM_LIB_DECKLINK_LIBS)

if WINDOWS
gem_videoDECKLINK_la_LIBADD    += -lole32
gem_recordDECKLINK_la_LIBADD   += -lole32
endif

if GEM_DECKLINK_APISTUB
noinst_LTLIBRARIES += libDeckLinkAPIDispatch.la
gem_videoDECKLINK_la_LIBADD  += libDeckLinkAPIDispatch.la
gem_recordDECKLINK_la_LIBADD += libDeckLinkAPIDispatch.la
endif

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoDECKLINK_la_SOURCES = videoDECKLINK.cpp videoDECKLINK.h decklink_common.h
gem_recordDECKLINK_la_SOURCES= recordDECKLINK.cpp recordDECKLINK.h decklink_common.h

libDeckLinkAPIDispatch_la_SOURCES  = libDeckLinkAPIDispatch.cpp
if WINDOWS
libDeckLinkAPIDispatch_la_SOURCES += libDeckLinkAPI_i.c
endif WINDOWS
