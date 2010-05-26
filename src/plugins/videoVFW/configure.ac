AC_PREREQ(2.60)
AC_INIT([videoVFW],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoVFW.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


AC_CHECK_HEADERS([vfw.h])
GEM_CHECK_LIB(vfw32,vfw32,AVIFileInit,,,,video input)

# is this needed?
dnl GEM_CHECK_LIB(avicap32,avicap32,main,,,,video input)

AC_OUTPUT
