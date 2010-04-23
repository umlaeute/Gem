AC_PREREQ(2.60)
AC_INIT([filmQT4L],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([filmQT4L.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


GEM_CHECK_LIB(libquicktime,quicktime, quicktime_init,,,,[movie loader])
dnl lqt is the same as libquicktime
dnl this is double because there exists "lqt-config" and "pkg-config libquicktime"
dnl another option would have been to introduce aliases for packages
GEM_CHECK_LIB(lqt,quicktime, quicktime_init,,,,[movie loader])


AC_OUTPUT
