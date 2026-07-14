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

licenses = \
	LICENSES/Autoconf-exception-generic.txt \
	LICENSES/Autoconf-exception-macro.txt \
	LICENSES/BSD-3-Clause.txt \
	LICENSES/BSL-1.0.txt \
	LICENSES/Bitstream-Vera.txt \
	LICENSES/GPL-2.0-or-later.txt \
	LICENSES/GPL-3.0-or-later.txt \
	LICENSES/LGPL-2.1-or-later.txt \
	LICENSES/LicenseRef-BSD-3-Clause-endorsement-variant.txt \
	LICENSES/LicenseRef-GPL3-linking-exception-AVT.txt \
	LICENSES/LicenseRef-GPL3-linking-exception-DECKLINK.txt \
	LICENSES/LicenseRef-GPL3-linking-exception-HALCON.txt \
	LICENSES/LicenseRef-GPL3-linking-exception-NDI.txt \
	LICENSES/LicenseRef-GPL3-linking-exception-PYLON.txt \
	LICENSES/LicenseRef-VIDS.txt \
	LICENSES/MIT.txt \
	LICENSES/SGI-B-2.0.txt \
	LICENSES/Unlicense.txt \
	$(nil)

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

EXTRA_DIST = \
	autogen.sh \
	REUSE.toml \
	$(licenses) \
	$(nil)
