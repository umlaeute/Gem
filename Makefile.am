AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS =src abstractions
SUBDIRS+=help doc examples
SUBDIRS+=build

## plugins
SUBDIRS+=plugins

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
	README.txt \
	examples/data/vera.ttf \
	$(nil)

EXTRA_DIST = autogen.sh
