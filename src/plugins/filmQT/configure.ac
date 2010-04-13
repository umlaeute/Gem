AC_PREREQ(2.57)
AC_INIT([filmQT],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_MACRO_DIR([../../m4])
AC_CONFIG_SRCDIR([filmQT.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)

AC_MSG_WARN([no check for QuickTime yet (w32 & OSX])

dnl some checks for osX (is this enough?)
GEM_CHECK_FRAMEWORK(Carbon, 
                        [have_carbon_framework="yes"], 
                        [have_carbon_framework="no"])
GEM_CHECK_FRAMEWORK(QuickTime,
                        [have_quicktime_framework="yes"], 
                        [have_quicktime_framework="no"])



AC_OUTPUT
