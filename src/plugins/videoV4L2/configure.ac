AC_PREREQ(2.60)
AC_INIT([videoV4L2],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoV4L2.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)

GEM_ARG_WITH(v4l, [video input], [force])

if test "x$with_v4l" != "xno"; then
  AC_CHECK_HEADER(linux/videodev.h,
                  [
                    AC_DEFINE([HAVE_VIDEO4LINUX], [], [video capturing using v4l])
                    have_v4l="yes"
                  ],[have_v4l="no"])

# these two are quite the same...
 GEM_CHECK_LIB(libv4l2, v4l2, v4l2_open,,,,[video input])
fi


AC_OUTPUT
