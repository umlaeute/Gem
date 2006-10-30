#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.57)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)

AC_LANG(C++)

ARCH=$(uname -m)
KERN=$(uname -s)

GEM_CONFIGFILE=Base/config${KERN}.h
AC_CHECK_FILES([${GEM_CONFIGFILE}.in],, [GEM_CONFIGFILE="Base/configGeneric.h" DEFINES="$DEFINES -DHAVE_CONFIG_GENERIC_H"])

AC_CONFIG_HEADER([$GEM_CONFIGFILE])
AC_CONFIG_SRCDIR([Base/GemBase.h])

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
AC_SUBST(CONFIGUREFLAGS)

## store the flags passed to us
## is there no way to get the flags without quotes?
#CONFIGUREFLAGS=${ac_configure_args}
## and is this solution portable? time will show....
CONFIGUREFLAGS=$(echo ${ac_configure_args} | sed "s/'//g")


# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

GEM_CHECK_CXXFLAGS([-fPIC])


GEM_ARG_ENABLE([mmx],     [MMX-support])
dnl disabled SSE2 by default, since there are still few chips around that fully support it
dnl unlike MMX or SSE
GEM_ARG_DISABLE([sse2],    [SSE2-support])
GEM_ARG_ENABLE([altivec], [AltiVec-support])
GEM_ARG_DISABLE([powerpc], [PowerPC specific optimizations])

GEM_ARG_ENABLE([pascalstrings], [containing '\p'])

GEM_ARG_ENABLE([ARB], [openGL-ARB-extension support], [force])
GEM_ARG_ENABLE([NV], [openGL-NV-extension support], [force])
GEM_ARG_ENABLE([registerstruct], [use \"-freg-struct-return\" (needed for FreeFrame)], [force])
GEM_ARG_ENABLE([filmnew], [all in one version of pix_film/pix_video], [force])

AC_ARG_WITH([pd], 
	        AC_HELP_STRING([--with-pd=<path/to/pd>],[where to find pd-binary (./bin/pd.exe) and pd-sources]))
AC_ARG_WITH([pdversion], 
		AC_HELP_STRING([--with-pdversion=<ver>],[enforce a certain pd-version (e.g. 0.37)]))
AC_ARG_WITH([extension], 
		AC_HELP_STRING([--with-extension=<ext>],[enforce a certain file-extension (e.g. pd_linux)]))

GEM_TARGETS=Base
GEM_TARGET(Controls)
GEM_TARGET(Geos)
GEM_TARGET(Manips)
GEM_TARGET(Nongeos)
GEM_TARGET(Particles)
GEM_TARGET(Pixes)
GEM_TARGET(openGL)
GEM_TARGET_DISABLED(Vertex)


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


GEM_CHECK_FRAMEWORK(OpenGL,
        [have_opengl_framework="yes" have_gl="yes"],
        [have_opengl_framework="no" have_gl="no"])

# don't need libGL if we have the OpenGL framework
if test "x$have_opengl_framework" != "xyes"; then
 AC_CHECK_LIB(GL,glInitNames)
 if test "x$ac_cv_lib_GL_glInitNames" = "xyes"; then
   have_gl="yes"
 fi

 AC_CHECK_LIB(GLU,gluLookAt)
 if test "x$ac_cv_lib_GLU_gluLookAt" = "xyes"; then
   have_glu="yes"
 fi
fi

GEM_CHECK_FRAMEWORK(AGL)
if test "x$gem_check_ldflags_success" = "xyes"; then
  have_glu="yes"
fi

dnl some checks for osX
GEM_CHECK_FRAMEWORK(Carbon, 
                        [have_carbon_framework="yes"], 
                        [have_carbon_framework="no"])
GEM_CHECK_FRAMEWORK(QuickTime,
                        [have_quicktime_framework="yes"], 
                        [have_quicktime_framework="no"])


# when using QuickTime framework, we don't want/need libquicktime
if test "x$have_quicktime_framework" = "xyes"; then
 if test "x$with_lqt" = "x"; then
   with_lqt="no"
 fi
 if test "x$with_libquicktime" = "x"; then
   with_libquicktime="no"
 fi
fi

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

# this is rather experimental...
if test "x$with_ffmpeg" != "xno" && \
        test "x${PKG_FFMPEG_CFLAGS}x${PKG_FFMPEG_LIBS}x" = "xxx" && \
        which ffmpeg-config > /dev/null
then
 PKG_FFMPEG_CFLAGS=$(ffmpeg-config --cflags)
 PKG_FFMPEG_LIBS=$(ffmpeg-config --plugin-libs avformat)
 if test "x$PKG_FFMPEG_CFLAGS" != "x"
 then
  ac_cv_env_PKG_FFMPEG_CFLAGS_set="set"
  ac_cv_env_PKG_FFMPEG_CFLAGS_value=${PKG_FFMPEG_CFLAGS}
 fi
 if test "x$PKG_FFMPEG_LIBS" != "x"
 then
  ac_cv_env_PKG_FFMPEG_LIBS_set="set"
  ac_cv_env_PKG_FFMPEG_LIBS_value="${PKG_FFMPEG_LIBS}"
 fi
fi

GEM_CHECK_LIB(ffmpeg, avformat, av_open_input_file,[PKG_CFLAGS="-I/usr/include/ffmpeg $PKG_CFLAGS"],,,[movie loader])

dnl checking whether (AVStream)->codec is "AVCodecContext" or "*AVCodecContext"
if test "x$have_ffmpeg" = "xyes"; then
 AC_MSG_CHECKING([whether (AVStream)->codec is of type \"*AVCodecContext\"])
 cflags_temp="$CXXFLAGS"
 CXXFLAGS="$CXXFLAGS $PKG_CFLAGS $PKG_FFMPEG_CFLAGS"
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
 CXXFLAGS=$cflags_temp
fi



GEM_ARG_WITH(v4l, [video input], [force])
GEM_ARG_WITHOUT(v4l2, [video input (might override v4l-1)], [force])

if test "x$with_v4l2" != "xno"; then
  AC_CHECK_HEADER(linux/videodev2.h,
                  [
                    AC_DEFINE(HAVE_VIDEO4LINUX2)
                    have_v4l2="yes"
                  ],[have_v4l2="no"], 
                  [
# include <asm/types.h>
# include <sys/time.h>
                  ])
fi

if test "x$with_v4l" != "xno"; then
  AC_CHECK_HEADER(linux/videodev.h,
                  [
                    AC_DEFINE(HAVE_VIDEO4LINUX)
                    have_v4l="yes"
                  ],[have_v4l="no"])
fi

GEM_CHECK_LIB(ieee1394,dv,main,,,,video input)

# this is rather experimental...and magic...uähh
if test "x$with_ftgl" != "xno" && \
        test "x${PKG_FTGL_CFLAGS}x${PKG_FTGL_LIBS}x" = "xxx" && \
        which pkg-config > /dev/null && \
        pkg-config --exists ftgl > /dev/null
then
 have_ftgl="no"
 PKG_FTGL_CFLAGS=$(pkg-config --cflags ftgl)
 PKG_FTGL_LIBS=$(pkg-config --libs ftgl | sed 's/-lftgl/-lftgl_pic/g')
 GEM_CHECK_LDFLAGS([${PKG_FTGL_LIBS}], [have_ftgl="yes"])

 if test "x$have_ftgl" = "xyes"
 then
  ac_cv_env_PKG_FTGL_CFLAGS_set="set"
  ac_cv_env_PKG_FTGL_CFLAGS_value=${PKG_FTGL_CFLAGS}
  ac_cv_env_PKG_FTGL_LIBS_set="set"
  ac_cv_env_PKG_FTGL_LIBS_value=${PKG_FTGL_LIBS}
 fi

fi
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
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([bzero floor gettimeofday memset munmap pow sqrt strchr strdup strrchr strstr])

# enabling recording with quicktime4linux
gem_tmp_lqt_libs="$LIBS"
LIBS="$LIBS $PKG_LIBS"
AC_MSG_CHECKING(for lqt_add_video_track)
AC_LINK_IFELSE([AC_LANG_CALL([], [lqt_add_video_track])], 
        AC_DEFINE(HAVE_LQT_ADD_VIDEO_TRACK)
        AC_MSG_RESULT(yes),
        AC_MSG_RESULT(no))
LIBS="$gem_tmp_lqt_libs"

# disabling ARB-extensions
if test "x$enable_ARB" != "xno"; then
dnl AC_CHECK_FUNC(glBindProgramARB, enable_ARB="yes",enable_ARB="no")
AC_MSG_CHECKING(for glBindProgramARB)
AC_LINK_IFELSE([AC_LANG_CALL([], [glBindProgramARB])], 
        enable_ARB="yes", enable_ARB="no")
AC_MSG_RESULT([$enable_ARB])
fi

if test "x$enable_ARB" = "xno"; then
AC_MSG_RESULT([disabling ARB-extensions])
 AC_DEFINE_UNQUOTED(DONT_USE_ARB)
fi

# disabling NV-extensions
if test "x$enable_NV" != "xno"; then
dnl AC_CHECK_FUNC(glBindProgramNV, enable_NV="yes",enable_NV="no")
AC_MSG_CHECKING(for glBindProgramNV)
AC_LINK_IFELSE([AC_LANG_CALL([], [glBindProgramNV])], 
        enable_NV="yes", enable_NV="no")
AC_MSG_RESULT([$enable_NV])
fi

if test "x$enable_NV" = "xno"; then
AC_MSG_RESULT([disabling NV-extensions])
 AC_DEFINE_UNQUOTED(DONT_USE_NV)
fi

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

if test "x$with_pd" != "x"; then
   INCLUDES="$INCLUDES -I${with_pd}/src"
   LIBS="$LIBS -L${with_pd}/bin"
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
 AC_MSG_RESULT([using deprecated video-loader])
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

if test "x$KERN" = "xDarwin"; then
 GEM_CHECK_CXXFLAGS([-Os],,GEM_CHECK_CXXFLAGS([-O3]))
else
 GEM_CHECK_CXXFLAGS([-O3])
fi

dnl this is only useful for debugging and might break things on older compilers
dnl GEM_CHECK_CXXFLAGS([-fno-builtin])

GEM_CHECK_CXXFLAGS([-falign-loops=32 -falign-functions=32 -falign-jumps=32 -funroll-loops],,
        GEM_CHECK_CXXFLAGS([-falign-loops=16 -funroll-loops])
        )
GEM_CHECK_CXXFLAGS([-ffast-math])


## SIMD support
# naive approach is to just check, which SIMD-flags the compiler takes
# and use these
# drawback: while the compiler might support compilation for a specific
# SIMD instruction set, the hardware you are compiling for might not

# proposal:
# by default the configure-task should only enable flags that are known
# to work on the machine we are compiling.
# the user can override this by explicitely stating that they want the
# flag to be enabled (if possible)
# e.g. "./configure --enable-sse2" on a pentium-3 should
# produce a binary that will only run on machines with SSE2 registers
# whereas "./configure" will make a binary with only MMX enabled.
#
# but how do we test for the CPU-caps? on linux we could just read /proc/cpuinfo
# but on other OSs?

# we make it a bit complicated in order to allow people to enforce the
# compilation with a specific SIMD-instruction set, even on platforms
# that don't support it
# why? think cross-compilation

if test "x${ARCH}" = "xi386" || test "x${ARCH}" = "xi686" || test "x${ARCH}" = "xx86_64"; then
 if test "x$enable_altivec" != "xyes"; then enable_altivec="no"; fi
fi

if test "x${ARCH}" = "xPower Macintosh" || test "x${ARCH}" = "xpowerpc"; then
 if test "x$enable_mmx"  != "xyes"; then enable_mmx="no" ; fi
 if test "x$enable_sse2" != "xyes"; then enable_sse2="no"; fi
fi

if test "x$enable_mmx" != "xno"; then
  GEM_CHECK_CXXFLAGS([-mmmx])
fi
if test "x$enable_sse2" = "xyes"; then
  GEM_CHECK_CXXFLAGS([-msse2])
fi
if test "x$enable_altivec" != "xno"; then
  GEM_CHECK_CXXFLAGS([-faltivec])
fi
if test "x$enable_powerpc" = "xyes"; then
  GEM_CHECK_CXXFLAGS([-mpowerpc-gfxopt])
fi

# on os-x we might come across "pascal strings" containing "\p"...
if test "x$enable_pascalstrings" != "xno"; then
  GEM_CHECK_CXXFLAGS([-fpascal-strings])
fi


# damned: gcc on osX seems to report that it supports "-ip -ipo_obj" although they are only icc flags
dnl  GEM_CHECK_CXXFLAGS([-ip -ipo_obj])

#linking on linux
GEM_CHECK_LDFLAGS([-shared -Wl,-export-dynamic])
# linking on os-X
GEM_CHECK_LDFLAGS([-dynamiclib -mmacosx-version-min=10.3 -undefined dynamic_lookup],
                  ,
	          GEM_CHECK_LDFLAGS([-bundle -undefined suppress -flat_namespace])
                 )

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

if test "x$have_quicktime_framework" = "xyes"; then
 have_libquicktime="yes (framework)"
fi

if test "x$have_v4l2" = "xyes"; then
  have_v4l="yes (v4l-2)"
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
