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
AC_SUBST(CPPFLAGS)
AC_SUBST(DEFINES)
AC_SUBST(PKG_CFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(prefix)
AC_SUBST(REFERENCEPATH)
AC_SUBST(NEW_VIDEOFILM)
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


AC_ARG_WITH([ALL],
             AC_HELP_STRING([--without-ALL], [en/disable ALL libraries by default]))


GEM_ARG_ENABLE([PIC],     [PositionIndependentCode (potentially slower)])

if test "x$enable_PIC" != "xno"; then
 GEM_CHECK_CXXFLAGS([-fPIC])
fi

GEM_ARG_ENABLE([mmx],     [MMX-support])
dnl disabled SSE2 by default, since there are still few chips around that fully support it
dnl unlike MMX or SSE
GEM_ARG_DISABLE([sse2],    [SSE2-support])
GEM_ARG_ENABLE([altivec], [AltiVec-support])
GEM_ARG_DISABLE([powerpc], [PowerPC specific optimizations])

GEM_ARG_ENABLE([pascalstrings], [containing '\p'])

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

## http://wiki.fifengine.de/Segfault_in_cxa_allocate_exception#Workaround_.231
AC_CHECK_LIB(stdc++,main)

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
if test "x${have_ImageMagick}" = "xyes"
then
 with_tiff=no
 with_jpeg=no
fi
GEM_CHECK_LIB(tiff, tiff, TIFFOpen,,,,[image loader])
GEM_CHECK_LIB(jpeg, jpeg, jpeg_read_header,,,,[image loader])


GEM_CHECK_LIB(gmerlin_avdec, gmerlin_avdec, gavl_start,,,,[movie loader])
if test "x${have_gmerlin_avdec}" = "xyes"
then
 AC_MSG_RESULT([gmerlin was found...disabling other video-decoders by default])

## not disabling libquicktime, as this is needed for recording as well!
# if test "x${with_libquicktime}" = "x"; then
#   with_libquicktime="no"
# fi
# if test "x${with_lqt}" = "x"; then
#   with_lqt="no"
# fi

 if test "x${with_libmpeg3}" = "x"; then
   with_libmpeg3="no"
 fi
 if test "x${with_mpeg}" = "x"; then
   with_mpeg="no"
 fi
 if test "x${with_avifile}" = "x"; then
   with_avifile="no"
 fi
fi


GEM_CHECK_LIB(libquicktime,quicktime, quicktime_init,,,,[movie loader])
dnl lqt is the same as libquicktime
dnl this is double because there exists "lqt-config" and "pkg-config libquicktime"
dnl another option would have been to introduce aliases for packages
GEM_CHECK_LIB(lqt,quicktime, quicktime_init,,,,[movie loader])

GEM_CHECK_LIB(libmpeg3,mpeg3,mpeg3_check_sig,,,,[movie loader])
GEM_CHECK_LIB(mpeg,mpeg,OpenMPEG,,,,[movie loader])
GEM_CHECK_LIB(avifile, aviplay, CreateIAviReadFile,,,,[movie loader])

GEM_ARG_WITH(v4l, [video input], [force])

if test "x$with_v4l" != "xno"; then
  AC_CHECK_HEADER(linux/videodev.h,
                  [
                    AC_DEFINE(HAVE_VIDEO4LINUX)
                    have_v4l="yes"
                  ],[have_v4l="no"])

# these two are quite the same...
 GEM_CHECK_LIB(libv4l1, libv4l1, v4l1_open,,,,[video input])
 GEM_CHECK_LIB(libv4l2, libv4l2, v4l2_open,,,,[video input])
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
  [INCLUDES="$INCLUDES -I/usr/include/FTGL -I/usr/include/freetype2"],,[ -lfreetype], font support)


GEM_CHECK_LIB(artoolkit,AR, arInitCparam,,,,ARToolKit)

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
AC_MSG_CHECKING(for lqt_open_write)
AC_LINK_IFELSE([AC_LANG_CALL([], [lqt_open_write])], 
        AC_DEFINE(HAVE_LQT_OPEN_WRITE)
        AC_MSG_RESULT(yes),
        AC_MSG_RESULT(no))
LIBS="$gem_tmp_lqt_libs"

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
  REFERENCEPATH=extra/Gem
else
  REFERENCEPATH=doc/5.reference/Gem
fi

dnl LATER check why this doesn't use the --with-pd includes
dnl for now it will basically disable anything that needs s_stuff.h if it cannot be found in /usr[/local]/include
AC_CHECK_HEADER(s_stuff.h)


# using pix_filmNEW rather than pix_filmOS
if test "x$enable_filmnew" != "xno"; then
 AC_DEFINE_UNQUOTED(NEW_VIDEOFILM)
else
 AC_MSG_RESULT([using deprecated video-loader])
fi

have_gl_headers="no"
AC_CHECK_HEADER(GL/gl.h,have_gl_headers="yes")
AC_CHECK_HEADER(OpenGL/gl.h,have_gl_headers="yes")
if test "x$have_gl_headers" != "xyes"; then
  have_gl="no"
fi

have_glu_headers="no"
AC_CHECK_HEADER(GL/glu.h,have_glu_headers="yes")
AC_CHECK_HEADER(OpenGL/glu.h,have_glu_headers="yes")
if test "x$have_glu_headers" != "xyes"; then
  have_glu="no"
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

# shall we built FAT-binaries ?
## this needs more thinking! running just the preprocessor does not like these flags
## for now we could live with just adding these manually with GEM_EXTRA_CXXFLAGS and GEM_EXTRA_LDFLAGS
dnl GEM_CHECK_CXXFLAGS([-arch i386 -arch ppc], GEM_CHECK_LDFLAGS([-arch i386 -arch ppc], [EXT="d_fat"]))

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

GEM_CHECK_FAT(ARCH_FLAGS)
AC_MSG_RESULT($ARCH_FLAGS)
AC_SUBST(ARCH_FLAGS)

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
GEM_CHECK_LDFLAGS([-shared -Wl,--export-dynamic])
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

if test "x$have_ftgl" = "xyes"; then
  have_font=FTGL
fi

if test "x$have_quicktime_framework" = "xyes"; then
 have_libquicktime="yes (framework)"
fi

if test "x$have_libv4l1" = "xyes" || test "x$have_libv4l2" = "xyes"; then
  have_v4l="yes (libv4l)"
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
  extension              : ${EXT}

 used optional libraries:

  font-rendering         : ${have_font}

  image-support
    use ImageMagick      : ${have_ImageMagick}
    use TIFF             : ${have_tiff}
    use JPEG             : ${have_jpeg}
  video-support
    use mpeg             : ${have_mpeg}
    use mpeg-3           : ${have_libmpeg3}
    use QuickTime        : ${have_libquicktime}
    use aviplay          : ${have_avifile}
    use gmerlin          : ${have_gmerlin_avdec}
  input-support
    use v4l              : ${have_v4l}
    use ieee1394         : ${have_ieee1394}
])

if test "x$PD_VERSION" = "x"; then
  AC_MSG_ERROR([Pd (header) is mandatory and seems to be missing!])
fi

if test "x$have_gl" != "xyes"; then
 AC_MSG_ERROR([GL (headers) not found! you need openGL!!!])
fi

if test "x$have_glu" != "xyes"; then
 AC_MSG_ERROR([GLU (headers) not found! you need openGL Utility Library!!!])
fi

AC_MSG_RESULT([Now run make ...])
