
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src $(GEM_EXTERNAL_CPPFLAGS)

pkglib_LTLIBRARIES =
gemhelpdir=$(pkglibdir)
dist_gemhelp_DATA =

EXTRA_DIST =
EXTRA_DIST += LICENSE.txt README.txt
EXTRA_DIST += \
	NDI/Processing.NDI.utilities.h \
	NDI/Processing.NDI.structs.h \
	NDI/Processing.NDI.deprecated.h \
	NDI/Processing.NDI.compat.h \
	NDI/Processing.NDI.Send.h \
	NDI/Processing.NDI.Routing.h \
	NDI/Processing.NDI.Recv.h \
	NDI/Processing.NDI.Recv.ex.h \
	NDI/Processing.NDI.Lib.h \
	NDI/Processing.NDI.Lib.cplusplus.h \
	NDI/Processing.NDI.FrameSync.h \
	NDI/Processing.NDI.Find.h \
	NDI/Processing.NDI.DynamicLoad.h \
	$(empty)

if HAVE_NDI
pkglib_LTLIBRARIES+= gem_videoNDI.la gem_recordNDI.la
dist_gemhelp_DATA  += NDI-videoplugin.pd NDI-recordplugin.pd
endif

# videoNDI

AM_CXXFLAGS =
AM_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
AM_LDFLAGS += -no-undefined
endif
if MACOSX
# nothing todo on macOS
else
AM_LDFLAGS  += -Wl,-rpath='$$ORIGIN'
endif

# no extra libraries required (libNDI is opened at runtime)
gem_videoNDI_la_LIBADD   =
gem_recordNDI_la_LIBADD   =

# RTE
AM_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
AM_LDFLAGS  += $(GEM_RTE_LIBS)   $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
AM_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gem_videoNDI_la_LIBADD   += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
gem_recordNDI_la_LIBADD  += -L$(builddir)/../.. $(GEM_EXTERNAL_LIBS)
# #gem_videoNDI_la @MOREFLAGS@

# Dependencies
AM_CXXFLAGS += $(GEM_NDI_CXXFLAGS)
if GEM_NDI_LOCAL
AM_CXXFLAGS += -I$(srcdir)/NDI/
endif
gem_videoNDI_la_LIBADD   += $(GEM_NDI_LIBS)
gem_recordNDI_la_LIBADD   += $(GEM_NDI_LIBS)


# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoNDI_la_SOURCES= videoNDI.cpp videoNDI.h init_ndi_library.hh
gem_recordNDI_la_SOURCES= recordNDI.cpp recordNDI.h init_ndi_library.hh

