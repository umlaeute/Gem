AC_PREREQ(2.60)
AC_INIT([videoHALCON],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoHALCON.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


GEM_ARG_WITH(halcon, [video input], [force])

#g++ -O6 -march=pentium -mtune=pentiumpro -I/home/zmoelnig/halcon//include -I/home/zmoelnig/halcon//include/cpp  ./ean13.cpp -I/home/zmoelnig/halcon//include -I/home/zmoelnig/halcon//include/cpp \
#          -L/usr/X11R6/lib -lXext -lX11 -lpthread -lm -ldl -L/home/zmoelnig/halcon//lib/x86-linux2.4-gcc40/ -lhalconcpp -lhalcon -o ../bin/x86-linux2.4-gcc40/ean13

## CFLAGS: -I${HALCONROOT}/include -I${HALCONROOT}/include/cpp
## LDFLAGS: -L${HALCONROOT}/lib/${HALCONARCH} -lhalconcpp -lhalcon


if test "x$with_halcon" != "xno"; then
## this could be overridden with HALCONROOT


  if test -d "${with_halcon}/include" ; then
   AC_LIB_APPENDTOVAR([INCLUDES], "-I${with_halcon}/include")
   if test -d "${with_halcon}/include/cpp" ; then
    AC_LIB_APPENDTOVAR([INCLUDES], "-I${with_halcon}/include/cpp")
   fi
  fi

  AC_CHECK_HEADER(HalconCpp.h,
                  [
                    AC_DEFINE([HAVE_HALCON], [], [video capturing using MVtec's HALCON])
                    have_halcon="yes"
                  ],[have_halcon="no"])

 GEM_CHECK_LIB(libhalcon, halcon, grab_image,,,,[video input])
 GEM_CHECK_LIB(libhalconcpp, halconcpp, grab_image,,,,[video input (C++)])
fi



AC_OUTPUT
