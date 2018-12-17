#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(Gem-extras,[0.1],zmoelnig@iem.at)
AM_INIT_AUTOMAKE([1.10 foreign])

AC_CONFIG_MACRO_DIR([../m4])

AC_CONFIG_SRCDIR([README-EXTRA.txt])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_SUBST(CXX)

## optional components
AS_IF([test -d "$srcdir/pix_artoolkit"], [AC_CONFIG_SUBDIRS([pix_artoolkit])])

AC_OUTPUT

