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

if test "x$with_v4l" != "xno"; then
  AC_CHECK_HEADERS(linux/videodev.h,
                  [have_v4l="yes"],
		  [have_v4l="no"]
		  ) 
   GEM_CHECK_LIB(libv4l1, v4l1, v4l1_open,,,,[video output])
fi

AC_OUTPUT
