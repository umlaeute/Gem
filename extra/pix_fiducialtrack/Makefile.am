AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/src/m4
AM_CPPFLAGS = -I$(top_srcdir)/src
include ../convenience-link-extra.mk

EXTRA_DIST =
EXTRA_DIST+=win-vs2003/pix_fiducialtrack.sln win-vs2003/pix_fiducialtrack.vcproj
EXTRA_DIST+=win-vs2008/pix_fiducialtrack.sln win-vs2008/pix_fiducialtrack.vcproj

extradir=$(libdir)/pix_fiducialtrack

extra_LTLIBRARIES = pix_fiducialtrack.la
noinst_LTLIBRARIES = libfidtrack.la

dist_extra_DATA  =
dist_extra_DATA += pix_fiducialtrack-help.pd all.trees

## some default flags
pix_fiducialtrack_la_CXXFLAGS =
pix_fiducialtrack_la_LDFLAGS  = -module -avoid-version -shared -shrext .$(GEM_RTE_EXTENSION)
if WINDOWS
pix_fiducialtrack_la_LDFLAGS += -no-undefined
endif
pix_fiducialtrack_la_LIBADD   =

# RTE flags
pix_fiducialtrack_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
pix_fiducialtrack_la_LIBADD   += $(GEM_RTE_LIBS)

pix_fiducialtrack_la_LDFLAGS += $(GEM_ARCH_LDFLAGS)

# special flags for building externals
pix_fiducialtrack_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
pix_fiducialtrack_la_LIBADD += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)

pix_fiducialtrack_la_LIBADD   += libfidtrack.la

libfidtrack_la_CFLAGS  =
libfidtrack_la_CXXFLAGS  =
libfidtrack_la_LIBADD    =
libfidtrack_la_LDFLAGS   =

## libfidtrack links against RTE because it uses "error"
libfidtrack_la_CFLAGS   += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libfidtrack_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libfidtrack_la_LIBADD   += $(GEM_RTE_LIBS)
libfidtrack_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

### SOURCES
pix_fiducialtrack_la_SOURCES = \
	pix_fiducialtrack.cpp \
	pix_fiducialtrack.h

libfidtrack_la_SOURCES = \
	fidtrackX.c \
	fidtrackX.h \
	segment.c \
	segment.h \
	treeidmap.cpp \
	treeidmap.h \
	floatpoint.h \
	default_trees.h
