AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS =src abstractions
SUBDIRS+=help doc examples
SUBDIRS+=tools

## window objects
SUBDIRS+=src/Output

## plugins
if PLUGINS
SUBDIRS+=plugins
endif

## extra objects
SUBDIRS+=extra

#pkg-config support
#pkgconfigdir = ${exec_prefix}/lib/pkgconfig
pkgconfig_DATA = Gem.pc

gemdir=$(pkglibdir)

dist_gem_DATA = \
	Gem-meta.pd \
	AUTHORS \
	ChangeLog \
	COPYING.txt \
	GnuGPL.LICENSE.txt \
	README.deken.pd \
	README.md \
	examples/data/vera.ttf \
	$(nil)

EXTRA_DIST = autogen.sh
