AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/src/m4
AM_CPPFLAGS = -I$(top_srcdir)/src

EXTRA_DIST =
EXTRA_DIST+=win-vs2003/pix_hit.sln win-vs2003/pix_hit.vcproj
EXTRA_DIST+=win-vs2008/pix_hit.sln win-vs2008/pix_hit.vcproj

extradir=$(libdir)/pix_hit

extra_LTLIBRARIES = pix_hit.la

dist_extra_DATA  = LICENSE.txt
dist_extra_DATA += pix_hit-help.pd

## some default flags
pix_hit_la_CXXFLAGS =
pix_hit_la_LDFLAGS  = -module -avoid-version -shared -shrext .$(GEM_RTE_EXTENSION)
if WINDOWS
pix_hit_la_LDFLAGS += -no-undefined
endif
pix_hit_la_LIBADD   =

# RTE flags
pix_hit_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
pix_hit_la_LIBADD   += $(GEM_RTE_LIBS)

pix_hit_la_LDFLAGS += $(GEM_ARCH_LDFLAGS)

# special flags for building externals
pix_hit_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
pix_hit_la_LIBADD += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)


# convenience symlinks
## uha, this is ugly
pix_hit.$(GEM_RTE_EXTENSION): pix_hit.la
	rm -f $@
	test -f .libs/$@ && $(LN_S) .libs/$@ $@ || true

.PHONY: clean-conviencesymlink

clean-conviencesymlink:
	rm -f pix_hit.$(GEM_RTE_EXTENSION)


all-local:: pix_hit.$(GEM_RTE_EXTENSION)

clean-local:: clean-conviencesymlink


### SOURCES
pix_hit_la_SOURCES = \
	pix_hit.cpp \
	pix_hit.h

