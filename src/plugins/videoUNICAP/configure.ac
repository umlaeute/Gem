AC_PREREQ(2.60)
AC_INIT([videoUNICAP],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoUNICAP.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)

GEM_ARG_WITH(unicap, [video input], [force])

if test "x$with_unicap" != "xno"; then
 GEM_CHECK_LIB(libunicap, unicap, unicap_check_version,,,,[video input])

  AC_CHECK_HEADER([unicap/unicap.h],
                  [
                    AC_DEFINE([HAVE_UNICAP], [], [video capturing using UNICAP])
                    have_unicap="yes"
                  ],[have_unicap="no"])

fi


AC_OUTPUT
