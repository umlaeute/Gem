AC_PREREQ(2.60)
AC_INIT([videoDarwin],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoDarwin.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL

GEM_CHECK_RTE

AC_LANG(C++)

dnl some checks for osX (is this enough?)
GEM_CHECK_FRAMEWORK(Carbon, 
                        [have_carbon_framework="yes"], 
                        [have_carbon_framework="no"])
GEM_CHECK_FRAMEWORK(QuickTime,
                        [have_quicktime_framework="yes"], 
                        [have_quicktime_framework="no"])


AM_CONDITIONAL(VIDEODARWIN, test "x${have_carbon_framework}" = "xyes" && test "x${have_quicktime_framework}" = "xyes")

AC_OUTPUT
