AC_PREREQ(2.60)
AC_INIT([filmMPEG1],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_MACRO_DIR([../../m4])
AC_CONFIG_SRCDIR([filmMPEG1.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])



AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


GEM_CHECK_LIB(mpeg,mpeg,OpenMPEG,,,,[movie loader])


AC_OUTPUT
