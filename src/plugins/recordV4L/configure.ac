AC_PREREQ(2.60)
AC_INIT([recordV4L],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([recordV4L.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL

GEM_CHECK_RTE

AC_LANG(C++)

t "x$with_v4l" != "xno"; then
  AC_CHECK_HEADER(linux/videodev.h,
                  [
                    AC_DEFINE([HAVE_VIDEO4LINUX], [], [video recording using
v4l loopback devices])
                    have_v4l="yes"
                  ],[have_v4l="no"])
fi


AC_OUTPUT
