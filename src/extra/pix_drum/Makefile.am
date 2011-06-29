AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)

extradir=$(libdir)

extra_LTLIBRARIES = pix_drum.la

extra_DATA  = pix_drum-LICENSE.txt
extra_DATA += pix_drum-help.pd

## some default flags
pix_drum_la_CXXFLAGS =
pix_drum_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
pix_drum_la_LIBADD   =

# RTE flags
pix_drum_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
pix_drum_la_LIBADD   += @GEM_RTE_LIBS@

pix_drum_la_LDFLAGS += @GEM_ARCH_LDFLAGS@


## uha, this is ugly
pix_drum.@GEM_RTE_EXTENSION@: pix_drum.la
	rm -f $@
	test .libs/$@ && $(LN_S) .libs/$@ $@

all-local:: pix_drum.@GEM_RTE_EXTENSION@

pix_drum_la_SOURCES = \
	pix_drum.cpp \
	pix_drum.h

