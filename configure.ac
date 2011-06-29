#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([Gem],[0.93],[zmoelnig@iem.at],[Gem],[http://gem.iem.at])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_FILES([Makefile abstractions/Makefile])

# documentation
AC_CONFIG_FILES([help/Makefile doc/Makefile examples/Makefile])

AC_CONFIG_MACRO_DIR(src/m4)

#AC_CONFIG_SRCDIR([src/Base/GemBase.h])

AC_CONFIG_SUBDIRS([src])

GEM_CHECK_RTE()

AC_OUTPUT
