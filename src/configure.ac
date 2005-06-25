#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.



AC_PREREQ(2.57)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)

ARCH=$(uname -m)
KERN=$(uname -s)


GEM_ARG_ENABLE(mmx, MMX-support)
GEM_ARG_ENABLE(ARB, openGL-ARB-support)
GEM_ARG_ENABLE(registerstruct, [use \"-freg-struct-return\" (needed for FreeFrame)])
GEM_ARG_ENABLE(filmnew, [all in one version of pix_film/pix_video])

AC_ARG_WITH(pdversion, [  --with-pdversion=<ver>  enforce a certain pd-version (e.g. 0.37)])


GEM_WITH_TARGET(Base)
GEM_WITH_TARGET(Controls)
GEM_WITH_TARGET(Geos)
GEM_WITH_TARGET(Manips)
GEM_WITH_TARGET(MarkEx)
GEM_WITH_TARGET(Nongeos)
GEM_WITH_TARGET(Particles)
GEM_WITH_TARGET(Pixes)
GEM_WITH_TARGET(openGL)


AC_CONFIG_HEADER([Base/config$KERN.h])
AC_CONFIG_SRCDIR([Base/GemBase.h])

AC_SUBST(INCLUDES)
AC_SUBST(LIBS)
AC_SUBST(X_LIBS)
AC_SUBST(X_CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(DEFINES)
AC_SUBST(PKG_CFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(prefix)
AC_SUBST(GEM_TARGETS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

AC_PATH_XTRA
LIBS="$LIBS $X_LIBS"


AC_CHECK_LIB(m,sin)
AC_CHECK_LIB(z,main)
AC_CHECK_LIB(dl,dlopen)

AC_CHECK_LIB(X11,main)
AC_CHECK_LIB(Xext,main)
AC_CHECK_LIB(Xxf86vm,XF86VidModeGetAllModeLines)

AC_CHECK_LIB(Xext,main)

AC_CHECK_LIB(GL,glInitNames)
AC_CHECK_LIB(GLU,gluLookAt)

AC_CHECK_LIB(tiff, TIFFOpen)
AC_CHECK_LIB(jpeg, jpeg_read_header)
AC_CHECK_LIB(Magick++, main,
  [AC_DEFINE(HAVE_LIBMAGICKPLUSPLUS)]
  LIBS="-lMagick++ $LIBS")

AC_CHECK_LIB(quicktime, quicktime_init)
AC_CHECK_LIB(mpeg3,mpeg3_check_sig)
AC_CHECK_LIB(mpeg,OpenMPEG)

AC_CHECK_LIB(gltt, GLTTFont)


AC_CHECK_LIB(dv,main) 
AC_CHECK_LIB(ftgl,main,
  [AC_DEFINE(HAVE_LIBFTGL)]
  INCLUDES="$INCLUDES -I/usr/include/FTGL" && LIBS="-lftgl $LIBS")

# pkg-config checks

PKG_CHECK_MODULES(PKG_FTGL,ftgl)
PKG_CHECK_MODULES(PKG_FT2,freetype2)
PKG_CHECK_MODULES(PKG_AVI,avifile)
PKG_CHECK_MODULES(PKG_FFMPEG,ffmpeg,,[PKG2=""])

PKG_CHECK_MODULES(PKG_GLTT, gltt, [AC_DEFINE(HAVE_LIBGLTT)],[
	AC_CHECK_LIB(gltt, GLTTFont,
	  AC_DEFINE(HAVE_LIBGLTT)
	  INCLUDES="$INCLUDES -I/usr/include/gltt" && LIBS="-lgltt $LIBS")
])

# Checks for header files.
AC_PATH_X
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h float.h memory.h stddef.h stdlib.h string.h strings.h sys/ioctl.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([bzero floor gettimeofday memset munmap pow sqrt strchr strdup strrchr strstr])



### weird configuration hacks

# disabling ARB-extensions
if test "x$enable_ARB" = "x"; then
 AC_CHECK_LIB(GL,glBindProgramARB,
        enable_ARB="yes",enable_ARB="no")
fi

if test "x$enable_ARB" = "xno"; then
 echo "disabling ARB-extensions"
 AC_DEFINE_UNQUOTED(DONT_USE_ARB)
fi

# enabling "-freg-struct-return" (for FreeFrame)
# i wonder could we make this the default ???
if test "$enable_registerstruct" = "yes"; then
  echo "enabling \"register struct return\""
  echo "  allows to make use of FreeFrame-effects without recompiling them"
  echo "  might break things, if you are using several different compilers"
  CXXFLAGS="$CXXFLAGS -freg-struct-return"
else
  enable_registerstruct="no"
fi


AC_CONFIG_FILES([Make.config])
AC_OUTPUT

dnl echo GEM compilation options:
dnl echo =======================================
dnl echo
dnl echo FTGL support:         $ac_cv_lib_ftgl_main
dnl echo GLTT support:         $ac_cv_lib_gltt_GLTTFont
dnl echo libmpeg3 support:     $ac_cv_lib_mpeg3_main
dnl echo libquicktime support: $ac_cv_lib_quicktime_quicktime_init
dnl echo aviplay support:      $ac_cv_lib_aviplay_main
dnl echo libdv support:        $ac_cv_lib_dv_main
dnl echo
dnl echo ========================================


dnl
dnl give some feedback about the configuration
dnl
AC_MSG_RESULT([
Result:
  Target                 : Gem.${target}
  Objects                : ${GEM_TARGETS}

Configuration:
  Compiler               : ${CXX}
  CPPFLAGS               : ${CPPFLAGS}
  CXXFLAGS               : ${CXXFLAGS}
  optimization           : ${CXXOPTIMIZE_FLAGS}
  debugging              : ${CXXDEBUG_FLAGS}

  Install path           : ${prefix}

 pure-data:
  version                : ${PD_MAJORVERSION}.${PD_MINORVERSION}

 used optional libraries:

  font-rendering         : ${with_font}

  image-support
    use ImageMagick      : ${with_magick}
    use TIFF             : ${with_tiff}
    use JPEG             : ${with_jpeg}
  video-support
    use mpeg             : ${with_mpeg}
    use mpeg-3           : ${with_mpeg3}
    use QuickTime        : ${with_quicktime}
    use aviplay          : ${with_avi}
    use ffmpeg           : ${with_ffmpeg}
  input-support
    use v4l              : ${with_v4l}
    use ieee1394         : ${with_ieee1394}

  misc
    using ARB-extensions : ${enable_ARB}
    using reg-struct-ret : ${enable_registerstruct}

Now run make ...
])