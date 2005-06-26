#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.57)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)

ARCH=$(uname -m)
KERN=$(uname -s)

AC_CONFIG_HEADER([Base/config$KERN.h])
AC_CONFIG_SRCDIR([Base/GemBase.h])

AC_SUBST(GEM_TARGETS)
AC_SUBST(EXT)
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
AC_SUBST(STRIP)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

AC_PATH_XTRA
LIBS="$LIBS $X_LIBS"




GEM_ARG_ENABLE(SIMD, SIMD-support)
GEM_ARG_ENABLE(ARB, openGL-ARB-support)
GEM_ARG_ENABLE(registerstruct, [use \"-freg-struct-return\" (needed for FreeFrame)])
GEM_ARG_ENABLE(filmnew, [all in one version of pix_film/pix_video])

AC_ARG_WITH([pdversion], 
		AC_HELP_STRING([--with-pdversion=<ver>],[enforce a certain pd-version (e.g. 0.37)]))
AC_ARG_WITH([extension], 
		AC_HELP_STRING([--with-extension=<ext>],[enforce a certain file-extension (e.g. pd_linux)]))

GEM_WITH_TARGET(Base)
GEM_WITH_TARGET(Controls)
GEM_WITH_TARGET(Geos)
GEM_WITH_TARGET(Manips)
GEM_WITH_TARGET(MarkEx)
GEM_WITH_TARGET(Nongeos)
GEM_WITH_TARGET(Particles)
GEM_WITH_TARGET(Pixes)
GEM_WITH_TARGET(openGL)


AC_CHECK_LIB(m,sin)
AC_CHECK_LIB(z,main)
AC_CHECK_LIB(dl,dlopen)

AC_CHECK_LIB(X11,main)
AC_CHECK_LIB(Xext,main)
AC_CHECK_LIB(Xxf86vm,XF86VidModeGetAllModeLines)

AC_CHECK_LIB(Xext,main)

AC_CHECK_LIB(GL,glInitNames)
AC_CHECK_LIB(GLU,gluLookAt)

GEM_CHECK_FRAMEWORK(OpenGL)
GEM_CHECK_FRAMEWORK(AGL)


dnl some checks for osX
GEM_CHECK_FRAMEWORK(Carbon)
GEM_CHECK_FRAMEWORK(QuickTime)


GEM_CHECK_LIB(ImageMagick++, Magick++, main,AC_DEFINE(HAVE_LIBMAGICKPLUSPLUS),,,[image loader])
dnl  [AC_DEFINE(HAVE_LIBMAGICKPLUSPLUS)]
dnl  LIBS="-lMagick++ $LIBS")
GEM_CHECK_LIB(tiff, tiff, TIFFOpen,,,,[image loader])
GEM_CHECK_LIB(jpeg, jpeg, jpeg_read_header,,,,[image loader])

GEM_CHECK_LIB(quicktime,quicktime, quicktime_init,,,,[movie loader])
GEM_CHECK_LIB(mpeg3,mpeg3,mpeg3_check_sig,,,,[movie loader])
GEM_CHECK_LIB(mpeg,mpeg,OpenMPEG,,,,[movie loader])
GEM_CHECK_LIB(avifile, aviplay, CreateIAviReadFile,,,,[movie loader])
GEM_CHECK_LIB(ffmpeg, avformat, av_open_input_file,[PKG_CFLAGS="-I/usr/include/ffmpeg $PKG_CFLAGS"],,
		[-lavcodec],[movie loader])

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

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([bzero floor gettimeofday memset munmap pow sqrt strchr strdup strrchr strstr])



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

# disabling ARB-extensions
if test "x$enable_ARB" = "x"; then
 AC_CHECK_LIB(GL,glBindProgramARB,
        enable_ARB="yes",enable_ARB="no")
fi

if test "x$enable_ARB" = "xno"; then
AC_MSG_RESULT([disabling ARB-extensions])
 AC_DEFINE_UNQUOTED(DONT_USE_ARB)
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
  GEM_CHECK_CXXFLAGS([-ip -ipo_obj])
fi


GEM_CHECK_LDFLAGS([-shared -Wl,-export-dynamic])
# rather do "-bundle_loader /path/to/pd/bin/pd"
GEM_CHECK_LDFLAGS([-bundle -undefined suppress -flat_namespace])


dnl if we don't have $STRIP set, we set it to ${host}-strip or strip
if test "x$STRIP" = "x"
then
 if test "x$host" != "x"
 then
  STRIP=${host}-strip
 else
# we need a method to check arguments for arbitrary commands like strip
  STRIP="strip"
 fi
fi

if which ${STRIP} > /dev/null
then
  STRIP="${STRIP} --strip-uneeded"
else
  STRIP="echo fake strip"
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

dnl make sources

for j in ${GEM_TARGETS}
do
    echo Sources for $j
    cd $j
    echo SOURCES= \\ > Make.source
    for f in $(ls -b1 *.cpp 2>/dev/null )
    do
     echo  $f \\ >> Make.source
    done
    echo >> Make.source
    cd - > /dev/null 2>&1
done


dnl
dnl give some feedback about the configuration
dnl

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

  Strip			 : ${STRIP}

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
    use QuickTime        : ${have_quicktime}
    use aviplay          : ${have_avifile}
    use ffmpeg           : ${have_ffmpeg}
  input-support
    use v4l              : ${have_v4l}
    use ieee1394         : ${have_ieee1394}

  misc
    using ARB-extensions : ${enable_ARB}
    using reg-struct-ret : ${enable_registerstruct}


Now run make ...

])
