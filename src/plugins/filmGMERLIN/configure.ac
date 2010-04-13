AC_PREREQ(2.57)
AC_INIT([filmGMERLIN],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_MACRO_DIR([../../m4])
AC_CONFIG_SRCDIR([filmGMERLIN.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


GEM_CHECK_LIB(gmerlin_avdec, gmerlin_avdec, gavl_start,,,,[movie loader])


AC_OUTPUT
