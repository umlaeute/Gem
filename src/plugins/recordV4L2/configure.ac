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

if test "x$with_v4l2" != "xno"; then
  AC_CHECK_HEADER(linux/videodev2.h,
                  [
                    AC_DEFINE([HAVE_LINUX_VIDEODEV2_H], [], [video recording using v4l2 loopback devices])
                    have_v4l2="yes"
                  ],[have_v4l2="no"])
fi


AC_OUTPUT
