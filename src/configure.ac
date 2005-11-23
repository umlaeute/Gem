#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.57)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)

ARCH=$(uname -m)
KERN=$(uname -s)

AC_CONFIG_HEADER([Base/config$KERN.h])
AC_CONFIG_SRCDIR([Base/GemBase.h])

GEM_CONFIGFILE=Base/config${KERN}.h

AC_SUBST(CXX)
AC_SUBST(GEM_TARGETS)
AC_SUBST(EXT)
AC_SUBST(GEM_CONFIGFILE)
AC_SUBST(INCLUDES)
AC_SUBST(LIBS)
AC_SUBST(X_LIBS)
AC_SUBST(X_CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(DEFINES)
AC_SUBST(PKG_CFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(prefix)
AC_SUBST(REFERENCEPATH)
AC_SUBST(NEW_VIDEOFILM)
AC_SUBST(HAVE_GLSIZEIPTRARB)


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

GEM_ARG_ENABLE([SIMD], [SIMD-support])
GEM_ARG_ENABLE([ARB], [openGL-ARB-extension support])
GEM_ARG_ENABLE([NV], [openGL-NV-extension support])
GEM_ARG_ENABLE([registerstruct], [use \"-freg-struct-return\" (needed for FreeFrame)])
GEM_ARG_ENABLE([filmnew], [all in one version of pix_film/pix_video])

AC_ARG_WITH([pdversion], 
		AC_HELP_STRING([--with-pdversion=<ver>],[enforce a certain pd-version (e.g. 0.37)]))
AC_ARG_WITH([extension], 
		AC_HELP_STRING([--with-extension=<ext>],[enforce a certain file-extension (e.g. pd_linux)]))


AC_ARG_WITH([pd], 
		AC_HELP_STRING([--with-pd=<path/to/pd>],[pd-binary]))
if test "x$with_pd" != "x"; then
 PD_BINARY=$with_pd
else
 if test "x$PD_BINARY" = "x"; then
  PD_BINARY=$(which pd)
 fi
fi

GEM_TARGETS=Base
GEM_TARGET(Controls)
GEM_TARGET(Geos)
GEM_TARGET(Manips)
GEM_TARGET(MarkEx)
GEM_TARGET(Nongeos)
GEM_TARGET(Particles)
GEM_TARGET(Pixes)
GEM_TARGET(openGL)


AC_CHECK_LIB(m,sin)
AC_CHECK_LIB(z,main)
AC_CHECK_LIB(dl,dlopen)

# Checks for libraries.

## on OSX X shouldn't be used by default as there are other ways
## however, if people really want it ("--with-x") they should get it
if test "x$KERN" = "xDarwin"; then
 if test "x$with_x" = "x"; then
   with_x="no"
 fi
fi

AC_PATH_XTRA
LIBS="$LIBS $X_LIBS"

if test "x$with_x" != "xno"; then
 AC_CHECK_LIB(X11,main)
 AC_CHECK_LIB(Xext,main)
 AC_CHECK_LIB(Xxf86vm,XF86VidModeGetAllModeLines)

 AC_CHECK_LIB(Xext,main)
fi

have_gl="no"
have_glu="no"

AC_CHECK_LIB(GL,glInitNames)
if test "x$ac_cv_lib_GL_glInitNames" = "xyes"; then
  have_gl="yes"
fi

AC_CHECK_LIB(GLU,gluLookAt)
if test "x$ac_cv_lib_GLU_gluLookAt" = "xyes"; then
  have_glu="yes"
fi

GEM_CHECK_FRAMEWORK(OpenGL)
if test "x$gem_check_ldflags_success" = "xyes"; then
  have_gl="yes"
fi

GEM_CHECK_FRAMEWORK(AGL)
if test "x$gem_check_ldflags_success" = "xyes"; then
  have_glu="yes"
fi

dnl some checks for osX
GEM_CHECK_FRAMEWORK(Carbon)
GEM_CHECK_FRAMEWORK(QuickTime)

#fallback to imagemagick if people are too lazy to type ImageMagick
if test "x$with_ImageMagick" = "x"; then
 with_ImageMagick="$with_imagemagick"
fi

GEM_CHECK_LIB(ImageMagick++, Magick++, main,AC_DEFINE(HAVE_LIBMAGICKPLUSPLUS),,,[image loader])
dnl  [AC_DEFINE(HAVE_LIBMAGICKPLUSPLUS)]
dnl  LIBS="-lMagick++ $LIBS")
GEM_CHECK_LIB(tiff, tiff, TIFFOpen,,,,[image loader])
GEM_CHECK_LIB(jpeg, jpeg, jpeg_read_header,,,,[image loader])

GEM_CHECK_LIB(libquicktime,quicktime, quicktime_init,,,,[movie loader])
dnl lqt is the same as libquicktime
dnl this is double because there exists "lqt-config" and "pkg-config libquicktime"
dnl another option would have been to introduce aliases for packages
GEM_CHECK_LIB(lqt,quicktime, quicktime_init,,,,[movie loader])
GEM_CHECK_LIB(mpeg3,mpeg3,mpeg3_check_sig,,,,[movie loader])
GEM_CHECK_LIB(mpeg,mpeg,OpenMPEG,,,,[movie loader])
GEM_CHECK_LIB(avifile, aviplay, CreateIAviReadFile,,,,[movie loader])
dnl GEM_CHECK_LIB(avcodec, avcodec_pic, avcodec_close,,,,[movie loader])
GEM_CHECK_LIB(ffmpeg, avformat, av_open_input_file,[PKG_CFLAGS="-I/usr/include/ffmpeg $PKG_CFLAGS"],,,[movie loader])

dnl checking whether (AVStream)->codec is "AVCodecContext" or "*AVCodecContext"
if test "x$have_ffmpeg" = "xyes"; then
 AC_MSG_CHECKING([whether (AVStream)->codec is of type \"*AVCodecContext\"])
 cflags_temp="$CFLAGS"
 CFLAGS="$CFLAGS $PKG_CFLAGS"
 AC_COMPILE_IFELSE(
 AC_LANG_PROGRAM([[
#include "avformat.h"
#include "avcodec.h"
]],[[
AVStream*stream;
stream->codec->codec_id;
]]),
AC_DEFINE(GEM_AVSTREAM_CODECPOINTER)
AC_MSG_RESULT(yes),
AC_MSG_RESULT(no)
)
 CFLAGS=$cflags_temp
fi




GEM_ARG_WITH(v4l, [video input])
if test "x$with_v4l" != "xno"; then
  AC_CHECK_HEADER(linux/videodev.h,
                  [
                    AC_DEFINE(HAVE_VIDEO4LINUX)
                    have_v4l="yes"
                  ],[have_v4l="no"])
fi
GEM_CHECK_LIB(ieee1394,dv,main,,,,video input) 

GEM_CHECK_LIB(ftgl,ftgl,main,
  [INCLUDES="$INCLUDES -I/usr/include/FTGL -I/usr/include/freetype2"],,[ -lfreetpye], font support)

GEM_CHECK_LIB(gltt,gltt, GLTTFont,,,,font support (deprecated))


dnl Checking for `pthread_create' function in -pthread
AC_CHECK_LIB(pthread, pthread_create,[
                                      AC_DEFINE(HAVE_PTHREADS)
                                      LIBS="$LIBS -lpthread"],)


# pkg-config checks

dnl PKG_CHECK_MODULES(PKG_FT2,freetype2)

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

## check for GLsizeiptrARB
## uäh! isn't there something more cross-platform???
AC_CHECK_TYPES([GLsizeiptrARB],,
        ,[
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <OpenGL/glext.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glext.h>
#endif
]
        )


AC_CHECK_SIZEOF(unsigned int)
AC_CHECK_SIZEOF(void *)


# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([bzero floor gettimeofday memset munmap pow sqrt strchr strdup strrchr strstr])


### make-depend flags
if test "x$ac_cv_c_compiler_gnu" = "xyes"; then
    AC_SUBST(MAKEDEP_FLAGS, "-MM")
else
    AC_SUBST(MAKEDEP_FLAGS, "-M")
fi


### weird configuration hacks
# the extension
if test "x$with_extension" != "x"; then
 EXT=$with_extension
else
 EXT=pd_`echo $host_os | sed -e '/.*/s/-.*//' -e 's/\[.].*//'`

 if test "x$KERN" != "xDarwin";
 then
  if test "x$host_os" = "x"
  then
 dnl just assuming that it is still linux (e.g. x86_64)
   EXT="pd_linux"
  fi
 else
   EXT="pd_darwin"
 fi
fi

#pd version
AC_CHECK_HEADERS(m_pd.h)
# this is mainly interesting for the documentation
if test "x$with_pdversion" != "x"; then
  PD_VERSION="$with_pdversion"
else
cat > conftest.c << EOF
#include <stdio.h>
#include "m_pd.h"
int main(){
  printf("%d.%d\n", PD_MAJOR_VERSION, PD_MINOR_VERSION);
  return 0;
}
EOF
 if $CXX $INCLUDES -o conftest.o conftest.c > /dev/null 2>&1; then
  PD_VERSION=`./conftest.o`
 else
  PD_VERSION=""
 fi
fi

let PD_MAJORVERSION=`echo $PD_VERSION | cut -d"." -f1`+0
let PD_MINORVERSION=`echo $PD_VERSION | cut -d"." -f2`+0

if test "$PD_MAJORVERSION" -gt 0 -o "$PD_MINORVERSION" -ge 37; then
  REFERENCEPATH=extra/help-Gem
else
  REFERENCEPATH=doc/5.reference/Gem
fi

# using pix_filmNEW rather than pix_filmOS
if test "x$enable_filmnew" != "xno"; then
 AC_DEFINE_UNQUOTED(NEW_VIDEOFILM)
else
 echo "using deprecated video-loader"
fi

# disabling ARB-extensions
if test "x$enable_ARB" != "xno"; then
 AC_CHECK_FUNC(glBindProgramARB,
        enable_ARB="yes",enable_ARB="no")
fi

if test "x$enable_ARB" = "xno"; then
AC_MSG_RESULT([disabling ARB-extensions])
 AC_DEFINE_UNQUOTED(DONT_USE_ARB)
fi

# disabling NV-extensions
if test "x$enable_NV" != "xno"; then
 AC_CHECK_FUNC(glBindProgramNV,
        enable_NV="yes",enable_NV="no")
fi

if test "x$enable_NV" = "xno"; then
AC_MSG_RESULT([disabling NV-extensions])
 AC_DEFINE_UNQUOTED(DONT_USE_NV)
fi

# enabling "-freg-struct-return" (for FreeFrame)
# i wonder could we make this the default ???
if test "x$enable_registerstruct" != "xno"; then
GEM_CHECK_CXXFLAGS([-freg-struct-return],
		[enable_registerstruct="yes"], [enable_registerstruct="no"])

if test "x$enable_registerstruct" = "xyes"; then
 AC_MSG_RESULT([enabling "register struct return"
   allows to make use of FreeFrame-effects without recompiling them
   might break things, if you are using several different compilers])
fi
fi

GEM_CHECK_CXXFLAGS([-O3])

GEM_CHECK_CXXFLAGS([-fno-builtin])
GEM_CHECK_CXXFLAGS([-falign-loops=16 -funroll-loops])
GEM_CHECK_CXXFLAGS([-ffast-math])

if test "x$enable_SIMD" != "xno"; then
  GEM_CHECK_CXXFLAGS([-mmmx])
  GEM_CHECK_CXXFLAGS([-faltivec])
# damned: gcc on osX seems to report that it supports "-ip -ipo_obj" although they are only icc flags
dnl  GEM_CHECK_CXXFLAGS([-ip -ipo_obj])
fi


GEM_CHECK_LDFLAGS([-shared -Wl,-export-dynamic])
GEM_CHECK_LDFLAGS([-bundle -bundle_loader $(which pd)])

dnl if we don't have $STRIP set, we set it to ${host}-strip or strip
AC_CHECK_TOOL([STRIP], [strip], [true])
AC_MSG_CHECKING([if strip is GNU strip])
if $STRIP -V 2>&1 | grep GNU > /dev/null
then
    AC_SUBST(STRIP_ALL," -s")
    AC_SUBST(STRIP_UNNEEDED, "--strip-unneeded")
    AC_MSG_RESULT([yes])
else
    AC_SUBST(STRIP_ALL, "")
    AC_SUBST(STRIP_UNNEEDED,"-x")
    AC_MSG_RESULT([no])
fi

dnl we want dos2unix...
AC_CHECK_TOOL([DOS2UNIX], [dos2unix], [true])


AC_CONFIG_FILES([Make.config])
AC_OUTPUT

dnl GEM compilation options:
dnl =======================================
dnl
dnl give some feedback about the configuration
if test "x$have_gl" != "xyes"; then
 AC_MSG_ERROR([GL not found! you need openGL!!!])
fi

if test "x$have_ftgl" = "xyes"; then
  have_font=FTGL
elif test "x$have_gltt" = "xyes"; then
  have_font=GLTT
fi

AC_MSG_RESULT([
Result:
  Target                 : Gem.${EXT}
  Objects                : ${GEM_TARGETS}

Configuration:
  Compiler               : ${CXX}
  CXXFLAGS               : ${CXXFLAGS} ${X_CFLAGS}
			 : ${PKG_CFLAGS}
  INCLUDES               : ${INCLUDES}
  DEFINES		 : ${DEFINES}

  LIBS                   : ${LIBS}
			 : ${PKG_LIBS}
  LDFLAGS		 : ${LDFLAGS}
			 : ${X_LIBS}

  Strip			 : ${STRIP} ${STRIP_UNNEEDED}

  Install path           : ${prefix}

 pure-data:
  version                : ${PD_MAJORVERSION}.${PD_MINORVERSION}

 used optional libraries:

  font-rendering         : ${have_font}

  image-support
    use ImageMagick      : ${have_ImageMagick}
    use TIFF             : ${have_tiff}
    use JPEG             : ${have_jpeg}
  video-support
    use mpeg             : ${have_mpeg}
    use mpeg-3           : ${have_mpeg3}
    use QuickTime        : ${have_libquicktime}
    use aviplay          : ${have_avifile}
    use ffmpeg           : ${have_ffmpeg}
  input-support
    use v4l              : ${have_v4l}
    use ieee1394         : ${have_ieee1394}

  misc
    using ARB-extensions : ${enable_ARB}
    using NV-extensions  : ${enable_NV}
    using reg-struct-ret : ${enable_registerstruct}

Now run make ...

])
