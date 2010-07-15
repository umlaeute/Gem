AC_PREREQ(2.60)
AC_INIT([recordV4L2],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([recordV4L2.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL

GEM_CHECK_RTE

AC_LANG(C++)

AC_CHECK_HEADERS(linux/videodev2.h)

AC_OUTPUT
