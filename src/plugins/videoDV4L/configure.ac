AC_PREREQ(2.60)
AC_INIT([videoDV4L],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoDV4L.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


GEM_CHECK_LIB(libdv,dv,main,,,,video input)
GEM_CHECK_LIB(libraw1394,raw1394,main,,,,video input)
GEM_CHECK_LIB(libiec61883,iec61883,main,,,,video input)

AC_OUTPUT
