AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS =src abstractions
SUBDIRS+=help doc examples
SUBDIRS+=tools

## plugins
if PLUGINS
SUBDIRS+=plugins
endif

## window objects
SUBDIRS+=src/Output

## extra objects
SUBDIRS+=extra

#pkg-config support
#pkgconfigdir = ${exec_prefix}/lib/pkgconfig
pkgconfig_DATA = Gem.pc

gemdir=$(pkglibdir)

dist_gem_DATA = \
	Gem-meta.pd \
	ChangeLog \
	COPYING.txt \
	GnuGPL.LICENSE.txt \
	README.md \
	examples/data/vera.ttf \
	$(nil)

EXTRA_DIST = autogen.sh
