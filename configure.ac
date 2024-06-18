#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT([Gem],[0.94],[zmoelnig@iem.at],[Gem],[http://gem.iem.at])

AM_INIT_AUTOMAKE([1.10 foreign])

PKG_INSTALLDIR(['${exec_prefix}/lib/pkgconfig'])


#
AC_CONFIG_FILES([Makefile abstractions/Makefile])

# documentation
AC_CONFIG_FILES([help/Makefile doc/Makefile examples/Makefile])

# src
AC_CONFIG_FILES([src/Makefile src/Gem/Makefile src/Utils/Makefile])
AC_CONFIG_FILES([src/RTE/Makefile src/Base/Makefile src/plugins/Makefile ])
AC_CONFIG_FILES([src/Output/Makefile ])
AC_CONFIG_FILES([src/deprecated/Makefile ])

## plugins that use this autoconf instance for configuration
AS_IF([test "$enable_plugins" != "no" ], [
  AC_CONFIG_FILES([plugins/Makefile])

  AC_CONFIG_FILES([plugins/ASSIMP2/Makefile])
  AC_CONFIG_FILES([plugins/ASSIMP3/Makefile])
  AC_CONFIG_FILES([plugins/AVI/Makefile])
  AC_CONFIG_FILES([plugins/AVIPLAY/Makefile])
  AC_CONFIG_FILES([plugins/DC1394/Makefile])
  AC_CONFIG_FILES([plugins/DECKLINK/Makefile])
  AC_CONFIG_FILES([plugins/DV4L/Makefile])
  AC_CONFIG_FILES([plugins/FFMPEG/Makefile])
  AC_CONFIG_FILES([plugins/GMERLIN/Makefile])
  AC_CONFIG_FILES([plugins/imageIO/Makefile])
  AC_CONFIG_FILES([plugins/imageMAGICK/Makefile])
  AC_CONFIG_FILES([plugins/JPEG/Makefile])
  AC_CONFIG_FILES([plugins/MPEG1/Makefile])
  AC_CONFIG_FILES([plugins/MPEG3/Makefile])
  AC_CONFIG_FILES([plugins/NDI/Makefile])
  AC_CONFIG_FILES([plugins/OBJ/Makefile])
  AC_CONFIG_FILES([plugins/OptiTrack/Makefile])
  AC_CONFIG_FILES([plugins/PNM/Makefile])
  AC_CONFIG_FILES([plugins/PIPEWIRE/Makefile])
  AC_CONFIG_FILES([plugins/QT4L/Makefile])
  AC_CONFIG_FILES([plugins/QuickTime/Makefile])
  AC_CONFIG_FILES([plugins/SGI/Makefile])
  AC_CONFIG_FILES([plugins/STB/Makefile])
  AC_CONFIG_FILES([plugins/TEST/Makefile])
  AC_CONFIG_FILES([plugins/TIFF/Makefile])
  AC_CONFIG_FILES([plugins/UNICAP/Makefile])
  AC_CONFIG_FILES([plugins/V4L/Makefile])
  AC_CONFIG_FILES([plugins/V4L2/Makefile])
  AC_CONFIG_FILES([plugins/VFW/Makefile])
  AC_CONFIG_FILES([plugins/VIDS/Makefile])
  AC_CONFIG_FILES([plugins/VLC/Makefile])
  AC_CONFIG_FILES([plugins/VNC/Makefile])

  AC_CONFIG_FILES([plugins/filmAVF/Makefile])
  AC_CONFIG_FILES([plugins/filmDS/Makefile])
  AC_CONFIG_FILES([plugins/filmDSATL/Makefile])
  AC_CONFIG_FILES([plugins/filmDarwin/Makefile])

  AC_CONFIG_FILES([plugins/videoAVF/Makefile])
  AC_CONFIG_FILES([plugins/videoDS/Makefile])
  AC_CONFIG_FILES([plugins/videoDarwin/Makefile])
  AC_CONFIG_FILES([plugins/videoSGI/Makefile])
])
AM_CONDITIONAL([PLUGINS], [test "${enable_plugins}" != "no"])



## extra holds additional objects/libraries
# some of them have their own build system
#AC_CONFIG_FILES([extra/Makefile])
# some do not
AC_CONFIG_FILES([extra/pix_drum/Makefile extra/pix_mano/Makefile])
AC_CONFIG_FILES([extra/pix_fiducialtrack/Makefile])
AC_CONFIG_FILES([extra/pix_hit/Makefile])

# other stuff
AC_CONFIG_FILES([tools/Makefile])
AC_CONFIG_FILES([Gem.pc Gem-meta.pd])
AC_CONFIG_FILES([devgem.sh], [chmod +x devgem.sh])
AC_CONFIG_FILES([switch-gemwin.sh], [chmod +x switch-gemwin.sh])


##########################

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_HEADERS([src/Gem/GemGLconfig.h])
AC_CONFIG_SRCDIR([src/Base/GemBase.h])

AC_CONFIG_SUBDIRS([extra])
AS_IF([test "$enable_plugins" != "no" ], [
  AS_IF([test -d "$srcdir/plugins/AVT"], [AC_CONFIG_SUBDIRS([plugins/AVT])])
  AS_IF([test -d "$srcdir/plugins/HALCON"], [AC_CONFIG_SUBDIRS([plugins/HALCON])])
  AS_IF([test -d "$srcdir/plugins/PYLON"], [AC_CONFIG_SUBDIRS([plugins/PYLON])])
])

AC_LANG(C++)

ARCH=$(uname -m)
KERN=$(uname -s)

IFEQ=ifeq
IFNEQ=ifneq
IFDEF=ifdef
IFNDEF=ifndef
ELSE=else
ENDIF=endif
AC_SUBST([IFEQ])
AC_SUBST([IFNEQ])
AC_SUBST([IFDEF])
AC_SUBST([IFNDEF])
AC_SUBST([ELSE])
AC_SUBST([ENDIF])

AC_SUBST(CXXCPP)
AC_SUBST(CXX)
AC_SUBST(GEM_TARGETS)
AC_SUBST(EXT)
AC_SUBST(LIBS)

AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(DEFS)
AC_SUBST(DEFINES)

AC_SUBST(X_CFLAGS)
AC_SUBST(X_PRE_LIBS)
AC_SUBST(X_LIBS)
AC_SUBST(X_EXTRA_LIBS)

AC_SUBST(GEM_CHECK_LIB_CFLAGS)
AC_SUBST(GEM_CHECK_LIB_LIBS)
AC_SUBST(prefix)

# Checks for programs.
AC_PROG_CXX
## older versions of autotools lack tests for ObjC/ObjC++
## so we only run these if the m4-macros are defined
m4_ifdef([AC_PROG_OBJC], [AC_PROG_OBJC])
m4_ifdef([AC_PROG_OBJCXX], [AC_PROG_OBJCXX])

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_SED


AC_ARG_ENABLE([debug],
        AS_HELP_STRING([--enable-debug=FACILITIES],
                [enable additional debugging for the named FACILITIES (comma-separated);
                known facilities are 'pixconvert'. Others may follow.
                ]))


AC_ARG_WITH([ALL],
             AS_HELP_STRING([--without-ALL],[en/disable ALL libraries by default]))


GEM_CHECK_RTE()

AC_SUBST(GEM_CPPFLAGS)
AC_SUBST(GEM_CFLAGS)
AC_SUBST(GEM_CXXFLAGS)
AC_SUBST(GEM_LIBS)

GEM_CHECK_WERROR
GEM_CPPFLAGS="${GEM_RTE_CPPFLAGS}"
GEM_CHECK_EXTERNAL

# additional debugging
gem_debug_pixconvert=""
AS_CASE([$enable_debug],
        [yes], [
            GEM_CPPFLAGS="${GEM_CPPFLAGS} -DGEM_DEBUG_PIXCONVERT=1"
        ], [ # fallback
            AS_CASE([,"${enable_debug}",], [,pixconvert,], [GEM_CPPFLAGS="${GEM_CPPFLAGS} -DGEM_DEBUG_PIXCONVERT=1"])
        ])


AC_ARG_WITH([GLU], AC_HELP_STRING([--without-GLU], [force building without GLU (*not* recommended!)]))
GEM_ARG_ENABLE([mmx],     [MMX-support])
dnl disabled SSE2 by default, since there are still few chips around that fully support it
dnl unlike MMX or SSE
GEM_ARG_DISABLE([sse2],    [SSE2-support])
GEM_ARG_ENABLE([altivec], [AltiVec-support])
GEM_ARG_DISABLE([powerpc], [PowerPC specific optimizations])

GEM_ARG_ENABLE([pascalstrings], [containing '\p'])

GEM_ARG_ENABLE([registerstruct], [use \"-freg-struct-return\" (needed for FreeFrame)], [force])


GEM_TARGET(Controls)
GEM_TARGET(Geos)
GEM_TARGET(Manips)
GEM_TARGET(Nongeos)
GEM_TARGET(Particles)
GEM_TARGET(Pixes)
GEM_TARGET(openGL)
GEM_TARGET_DISABLED(Vertex)
GEM_ARG_ENABLE([plugins], [configure/build/install optional parts])

AC_ARG_ENABLE([experimental], AC_HELP_STRING([--enable-experimental], [enable experimental objects (if any)]))
AM_CONDITIONAL(EXPERIMENTAL, [test "x$enable_experimental" = "xyes"])


# checks for default headers
IEM_CHECK_INCLUDES_DEFAULT([windows.h])

# threading
GEM_CHECK_THREADS

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h unistd.h])
AC_CHECK_HEADERS([fcntl.h float.h memory.h string.h strings.h])
AC_CHECK_HEADERS([sys/select.h sys/ioctl.h sys/time.h sys/ipc.h sys/shm.h])

AC_CHECK_HEADERS([wordexp.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([void *])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([bzero floor gettimeofday memset munmap pow sqrt strchr strdup strrchr strstr strnlen])

# Checks for libraries.

AC_CHECK_LIB([m],[sin])
AC_CHECK_LIB([z],[zlibVersion])
AC_CHECK_LIB([dl],[dlopen])

## w32 compatibility library
#AC_CHECK_LIB([OLDNAMES], [close])
#AC_CHECK_LIB([coldname], [close])
## Standard C++ Library:
#AC_CHECK_LIB([libcpmt], [main])
## C runtime library:
#AC_CHECK_LIB([msvcrt], [fclose])
## Standard C++ Library:
#AC_CHECK_LIB([msvcprt], [main])

## on OSX X shouldn't be used by default as there are other ways
## however, if people really want it ("--with-x") they should get it
AS_IF([test "x$KERN" = "xDarwin" && test "x$with_x" = "x"], [with_x="no"])

AC_PATH_X
AC_PATH_XTRA

AS_IF([test "x$with_x" != "xno"], [
 AC_CHECK_LIB([Xxf86vm],[XF86VidModeGetAllModeLines])
 AC_CHECK_HEADERS([X11/extensions/XInput2.h])
 AC_CHECK_LIB([Xrender],[XRenderFindVisualFormat])
 AC_CHECK_HEADERS([X11/extensions/Xrender.h])
])

IEM_CHECK_GL
no_glu=yes
AS_IF([test "x$with_GLU" != "xno"], [IEM_CHECK_GLU])
AM_CONDITIONAL([HAVE_GLU], [test "x${no_glu}" != "xyes"])
IEM_CHECK_GLUT
IEM_CHECK_GLX
IEM_CHECK_AGL


AS_IF([test "x$no_glu" != "xyes"],
 AC_DEFINE([GEM_HAVE_GLU], [1], [whether we can use libGLU (normally: yes)])
)

## use system's libGLEW
GEM_ARG_ENABLE([multicontext], [experimental support for multiple windows])
## glewmx-args default to glew-args
AS_IF([test "x$with_glewmx" = "x"], [with_glewmx="$with_glew"])
AS_IF([test "x$with_glewmx-includes" = "x"], [with_glewmx-includes="$with_glew-includes"])
AS_IF([test "x$with_glewmx-libs" = "x"], [with_glewmx-libs="$with_glew-libs"])

GEM_CHECK_LIB([glew], [GLEW],[GL/glew.h], [glewInit],,,,[OpenGL Extension Wrangler library], [no])
GEM_CHECK_LIB([glewmx], [GLEWmx],[GL/glew.h], [glewInit],,,,[OpenGL Extension Wrangler library], [no])


AS_IF([test "x$enable_multicontext" = "xyes"], [
  have_glew="${have_glewmx}"
  GEM_LIB_GLEW_CFLAGS="$GEM_LIB_GLEWMX_CFLAGS"
  GEM_LIB_GLEW_LIBS="$GEM_LIB_GLEWMX_LIBS"

  AM_CONDITIONAL(HAVE_LIB_GLEW, [test "x${have_glew}" = "xyes"])
  AC_DEFINE([GEM_MULTICONTEXT], [1], [whether Gem is built with multi-context support])
])
AM_CONDITIONAL(ENABLE_MULTICONTEXT, [test "x${enable_multicontext}" = "xyes"])

AS_IF([test "x${have_glew}" != "xyes"], [
AC_DEFINE([GLEW_BUILD], [1], [Define to 1 if using the built-in glew])
])

GEM_CHECK_LIB([sdl], [SDL],[SDL.h], [SDL_Quit],,,,[SDL windowing])
GEM_CHECK_LIB([sdl2], [SDL2],[SDL_log.h], [SDL_Quit],,,,[SDL2 windowing])

## use GLFW windowing framework
GEM_CHECK_LIB([libglfw], [glfw],[GL/glfw.h], [glfwInit],,,,[GLFW2 windowing])
## use GLFW3 windowing framework
GEM_CHECK_LIB([glfw3], [glfw3],[GLFW/glfw3.h], [glfwGetPrimaryMonitor],,,,[GLFW3 windowing])


## http://wiki.fifengine.de/Segfault_in_cxa_allocate_exception#Workaround_.231
### "-lstdc++" really has to be added before e.g. "-lGL" in order to avoid
### triggering the bug
### however, libtool is way to clever and eliminates duplicate LIBS entries.
### since "-lstdc++" is usually found in the postdeps, this  would remove the
### entry found in LIBS; so we add it to the predeps_CXX (only)
stdcxx_LIBS=$LIBS
AC_CHECK_LIB([stdc++],[main],[predeps_CXX="$predeps_CXX -lstdc++"])
LIBS=$stdcxx_LIBS

dnl some checks for osX
dnl AC_CHECK_HEADERS([Cocoa/Cocoa.h],[], [ with_Cocoa_framework="no" ])
GEM_CHECK_FRAMEWORK([Cocoa],
                        [have_cocoa_framework="yes"],
                        [have_cocoa_framework="no"])
dnl AC_CHECK_HEADERS([AVFoundation/AVFoundation.h],[], [ with_AVFoundation_framework="no" ])
GEM_CHECK_FRAMEWORK([AVFoundation],
                        [have_avfoundation_framework="yes"],
                        [have_avfoundation_framework="no"])
GEM_CHECK_FRAMEWORK([ImageIO],
                        [have_imageio_framework="yes"],
                        [have_imageio_framework="no"])

AC_CHECK_HEADERS([Carbon/Carbon.h],[], [ with_Carbon_framework="no" ])
GEM_CHECK_FRAMEWORK([Carbon],
                        [have_carbon_framework="yes"],
                        [have_carbon_framework="no"])

have_quicktime_framework_headers="no"
AC_CHECK_HEADERS([QuickTime/QuickTime.h QTML.h], [have_quicktime_framework_headers="yes"])
AS_IF([test "x${have_quicktime_framework_headers}" != "xyes"], [with_QuickTime_framework="no"])
GEM_CHECK_FRAMEWORK([QuickTime],
                        [have_quicktime_framework="yes"],
                        [have_quicktime_framework="no"])

AM_CONDITIONAL(VIDEODARWIN, test "x${have_carbon_framework}" = "xyes" && test "x${have_quicktime_framework}" = "xyes")

#fallback to imagemagick if people are too lazy to type ImageMagick
AS_IF([test "x$with_ImageMagick" = "x"], [with_ImageMagick="$with_imagemagick"])
AS_IF([test "x$with_MagickCore" = "x"], [with_MagickCore="$with_ImageMagick"])

magickpackage=

GEM_CHECK_LIB([MagickCore],
              [Magick],[magick/MagickCore.h MagickCore/MagickCore.h],
              [IsMagickInstantiated],
              AC_DEFINE([HAVE_LIBMAGICK], [1], [image loading/saving with ImageMagick (Core)])
              magickpackage=MagickCore
              ,,,[image loader])
GEM_CHECK_LIB([ImageMagick++],
              [Magick++],[Magick++.h],
              [main],
              AC_DEFINE([HAVE_LIBMAGICKPLUSPLUS], [1], [image loading/saving with ImageMagick++])
              magickpackage=ImageMagick++
              ,,,[image loader])
magick7=no
AS_IF([test "x$magickpackage" != "x"], [
  PKG_CHECK_MODULES([MAGICK7], $magickpackage > 7,
    [AC_DEFINE([HAVE_MAGICK7], 1, [define if you have ImageMagick7 installed.])
     magick7=7
    ], [magick7=""])
])


GEM_CHECK_LIB([tiff], [tiff],[tiffio.h], [TIFFOpen],,,,[image loader])
GEM_CHECK_LIB([jpeg], [jpeg],[jpeglib.h], [jpeg_read_header],,,,[image loader])
GEM_CHECK_LIB([stb], [stb],[stb/stb_image.h], [stbi_load],,,,[image loader])

IEM_CHECK_FTGL()
GEM_CHECK_LIB([fribidi], [fribidi],[fribidi.h], [fribidi_log2vis],,,, [bidirectional text rendering using FriBidi])

## film en/decoders
GEM_CHECK_LIB([avifile], [aviplay],[avifile.h], [CreateIAviReadFile],,,,[movie loader])
GEM_CHECK_LIB([gmerlin_avdec], [gmerlin_avdec],[gmerlin/avdec.h], [gavl_start],,,,[movie loader])
AS_IF([test "x$have_gmerlin_avdec" = "xyes"], [
  AC_CHECK_HEADERS([gavl/log.h])
])
GEM_CHECK_LIB([mpeg], [mpeg],[mpeg.h], [OpenMPEG],,,,[movie loader])
GEM_CHECK_LIB([libmpeg3], [mpeg3],[libmpeg3.h], [mpeg3_check_sig],,,,[movie loader])
GEM_CHECK_LIB([libquicktime], [quicktime],[quicktime.h], [quicktime_init],,,,[movie loader])
GEM_CHECK_LIB([lqt], [quicktime],[quicktime.h], [quicktime_init],,,,[movie loader])

### ffmpeg is a bit complicated, as it depends on multiple libs
### for now, we only support pkg-config (or manual configuration)
GEM_ARG_WITH([ffmpeg], [movie loader])
AS_IF([ test "x${with_ffmpeg}" = "x" ], [ with_ffmpeg="${with_ALL}" ])
AS_IF([ test "x${with_ffmpeg}" = "xno" ], [
  have_ffmpeg="no (forced)"
],[
  have_ffmpeg="no (needs check)"
  PKG_CHECK_MODULES([FFMPEG], [libavformat libavutil libavcodec libswscale],
                    [have_ffmpeg=yes], [have_ffmpeg=no])
])
GEM_LIB_FFMPEG_CFLAGS=${FFMPEG_CFLAGS}
GEM_LIB_FFMPEG_LIBS=${FFMPEG_LIBS}
AC_SUBST([GEM_LIB_FFMPEG_CFLAGS])
AC_SUBST([GEM_LIB_FFMPEG_LIBS])
AM_CONDITIONAL(HAVE_FFMPEG, [test "x${have_ffmpeg}" = "xyes"])

# special function in libquicktime
old_cflags=${CFLAGS}
old_libs=${LIBS}
CFLAGS="${CFLAGS} $GEM_LIB_LIBQUICKTIME_CFLAGS  $GEM_LIB_LQT_CFLAGS"
LIBS="${LIBS}  $GEM_LIB_LIBQUICKTIME_LIBS  $GEM_LIB_LQT_LIBS"
AC_CHECK_FUNCS([lqt_seek_video])
AC_CHECK_FUNCS([lqt_open_write])
CFLAGS=${old_cflags}
LIBS=${old_libs}

#video
GEM_CHECK_LIB([libdc1394-2], [dc1394],[dc1394/dc1394.h],   [dc1394_new],,,, [video input])
GEM_CHECK_LIB([libdv],       [dv],[libdv/dv.h],       [dv_cleanup],,,, [video input])
GEM_CHECK_LIB([libraw1394],  [raw1394],[libraw1394/raw1394.h],  [raw1394_new_handle],,,, [video input])
GEM_CHECK_LIB([libiec61883], [iec61883],[libiec61883/iec61883.h], [iec61883_dv_fb_init],,,, [video input])
AC_CHECK_HEADERS([vl/vl.h])

GEM_ARG_WITH([unicap], [video input], [force])

GEM_CHECK_LIB([libunicap], [unicap],[unicap.h], [unicap_check_version],,,, [video input])

AS_IF([test "x$have_unicap" = "xyes"], [
 AC_CHECK_HEADER([unicap/unicap.h],
                  [
                    AC_DEFINE([HAVE_UNICAP], [], [video capturing using UNICAP])
                    have_unicap="yes"
                  ],[have_unicap="no"])
])
AS_IF([test "x$have_libunicap"], [have_unicap="yes"])
AM_CONDITIONAL([HAVE_LIB_UNICAP], [test "x${have_unicap}" = "xyes"])



# pipewire
AC_ARG_VAR([PKG_PIPEWIRE_CFLAGS], [C compiler flags for pipewire, overriding pkg-config])dnl
AC_ARG_VAR([PKG_PIPEWIRE_LIBS], [linker flags for pipewire, overriding pkg-config])dnl
AC_ARG_VAR([PKG_LIBPIPEWIRE_0_3_CFLAGS],[(Use PKG_PIPEWIRE_CFLAGS instead)])
AC_ARG_VAR([PKG_LIBPIPEWIRE_0_3_LIBS],[(Use PKG_PIPEWIRE_LIBS instead)])

AS_IF([test "x${PKG_PIPEWIRE_CFLAGS}" != x],
      [PKG_LIBPIPEWIRE_0_3_CFLAGS=${PKG_PIPEWIRE_CFLAGS}])
AS_IF([test "x${PKG_PIPEWIRE_LIBS}" != x],
      [PKG_LIBPIPEWIRE_0_3_LIBS=${PKG_PIPEWIRE_LIBS}])
GEM_ARG_WITH([pipewire], [video input/output], [force])
GEM_CHECK_LIB([libpipewire-0.3], [pipewire-0.3], [pipewire/pipewire.h], [pw_get_library_version],
              ,,, [video input/output], [$with_pipewire])
GEM_LIB_PIPEWIRE_CFLAGS=${GEM_LIB_LIBPIPEWIRE_0_3_CFLAGS}
GEM_LIB_PIPEWIRE_LIBS=${GEM_LIB_LIBPIPEWIRE_0_3_LIBS}
AC_SUBST([GEM_LIB_PIPEWIRE_CFLAGS])
AC_SUBST([GEM_LIB_PIPEWIRE_LIBS])
have_pipewire="${have_libpipewire_0_3}"
AM_CONDITIONAL([HAVE_PIPEWIRE], [test "x${have_pipewire}" = "xyes"])

have_v4l="no"
GEM_ARG_WITH([v4l], [video input], [force])
AC_CHECK_HEADERS([linux/videodev.h], have_v4l="yes")
GEM_CHECK_LIB([libv4l1], [v4l1],[libv4l1.h], [v4l1_open],,,, [video output])
AS_IF([test "x$have_libv4l1" = "xyes"], [have_v4l="yes (libv4l)"])
AS_IF([test "x$with_v4l" = "xno"], [have_v4l="no"])
AM_CONDITIONAL([HAVE_V4L], [test "x${have_v4l}" != "xno"])


have_v4l2="no"
GEM_ARG_WITH([v4l2], [video input], [force])
AC_CHECK_HEADERS([linux/videodev2.h])
GEM_CHECK_LIB([libv4l2], [v4l2],[libv4l2.h], [v4l2_open],,,, [video input])
# AC_DEFINE([HAVE_VIDEO4LINUX2], [], [video capturing using v4l2])

AS_IF([test "x$have_libv4l2" = "xyes"], [have_v4l2="yes (libv4l)"])
AS_IF([test "x$with_v4l2" = "xno"], [have_v4l2="no"])
AM_CONDITIONAL([HAVE_V4L2], [test "x${have_v4l2}" != "xno"])


dnl GEM_CHECK_LIB([vfw32], [vfw32],, [AVIFileInit],,,, [video input])
GEM_CHECK_LIB([vfw32], [vfw32],[vfw.h], [capGetVideoFormat],,,, [video input])
# is this needed?
dnl GEM_CHECK_LIB([avicap32],[avicap32],,[main],,,,[video input])
AM_CONDITIONAL([HAVE_VFW32], test "x${have_vfw32}" = "xyes")

# VLC
GEM_CHECK_LIB([libvlc], [vlc],[vlc/vlc.h], [libvlc_video_set_callbacks],,,, [video input])

# VNC
GEM_CHECK_LIB([libvncclient], [vncclient],[rfb/rfbclient.h], [rfbGetClient],,,, [video input])

# assimp
GEM_CHECK_LIB([assimp], [assimp],[assimp.h], [aiDetachAllLogStreams],,,, [3D model loading])
## check whether this is assimp2 or assimp3
have_assimp2="no"
have_assimp3="no"

old_cflags=${CFLAGS}
old_cxxflags=${CXXFLAGS}
old_ldflags=${LDFLAGS}
CFLAGS="${CFLAGS} ${GEM_LIB_ASSIMP_CFLAGS}"
CXXFLAGS="${CXXFLAGS} ${GEM_LIB_ASSIMP_CFLAGS}"
LDFLAGS="${LDFLAGS} ${GEM_LIB_ASSIMP_LIBS}"
AC_CHECK_HEADERS([assimp.h], [have_assimp2="yes"])
AC_CHECK_HEADERS([assimp/cimport.h], [have_assimp3="yes"])
CFLAGS=${old_cflags}
CXXFLAGS=${old_cxxflags}
LDFLAGS=${old_ldflags}

AM_CONDITIONAL([HAVE_ASSIMP2], [test "x${have_assimp2}" = "xyes" ])
AM_CONDITIONAL([HAVE_ASSIMP3], [test "x${have_assimp3}" = "xyes" ])

## text support
AC_ARG_WITH([default-font],
            AS_HELP_STRING([--with-default-font=vera.ttf],[specify the default font for text-rendering])
           )
AS_IF([test "x$with_default_font" != "x" && \
       test "x$with_default_font" != "xno" &&
       test "x$with_default_font" != "xyes"], [
  AC_DEFINE_UNQUOTED([GEM_DEFAULT_FONT], "${with_default_font}", [default font for text rendering])
  AC_MSG_RESULT([using "${with_default_font}" as default font])
])

# enabling recording with quicktime4linux
gem_tmp_lqt_libs="$LIBS"
LIBS="$LIBS $GEM_CHECK_LIB_LIBS"
AC_MSG_CHECKING(for lqt_open_write)
AC_LINK_IFELSE([AC_LANG_CALL([], [lqt_open_write])],
        AC_DEFINE([HAVE_LQT_OPEN_WRITE], [], [film recording using quicktime4linux])
        AC_MSG_RESULT(yes),
        AC_MSG_RESULT(no))
LIBS="$gem_tmp_lqt_libs"


# DECKLINK-support
GEM_DECKLINK_SYSTEM=""
AS_CASE([${host}]
        , [*-darwin*], [GEM_DECKLINK_SYSTEM=Darwin]
        , [*-linux*], [GEM_DECKLINK_SYSTEM=Linux]
        , [*mingw*], [GEM_DECKLINK_SYSTEM=Windows]
)
AC_SUBST([GEM_DECKLINK_SYSTEM])

have_decklink="no"
AS_IF([test "x$with_DeckLink" != "xno"], [
  tmp_decklink_includes=""
  AS_IF([test "x${with_DeckLink}" = "xyes" ], [ with_DeckLink="local" ])
  AS_IF([test "x${with_DeckLink}" = "x" ], [ with_DeckLink="local" ])
  AS_IF([test "x${with_DeckLink}" = "xlocal" ],  [
      decklink_sdkdir="${srcdir}/plugins/DECKLINK/SDK/${GEM_DECKLINK_SYSTEM}/"
      AS_IF([test -e ${decklink_sdkdir}/DeckLinkAPI.h], [
             with_DeckLink=yes
             AC_MSG_NOTICE([using embedded copy of DeckLink-SDK])
             have_decklink="local"
    ], [
             AC_MSG_WARN([requested to use embedded copy of DeckLink-SDK, but none found in ${decklink_sdkdir}])
             with_DeckLink=no
    ])
    ], [
      decklink_sdkdir=$with_DeckLink
      AS_IF([test -e ${decklink_sdkdir}/DeckLinkAPI.h], [
             with_DeckLink=yes
             with_DeckLink_CFLAGS="-I${decklink_sdkdir}"
             AC_MSG_NOTICE([using DeckLink-SDK in ${decklink_sdkdir}])
       ],[
             AC_MSG_WARN([requested to use DeckLink-SDK in ${decklink_sdkdir}, but none found])
             with_DeckLink=no
       ])
   ])
])
AM_CONDITIONAL([GEM_DECKLINK_LOCAL], [test "x${have_decklink}" = "xlocal"])

GEM_CHECK_LIB([DeckLink],[], [DeckLinkAPI.h],[CreateDeckLinkIteratorInstance], ,,,[video input; use 'local' to use an embedded copy of the SDK])
decklink_dlopen=
AS_IF([test "x${PKG_DECKLINK_LIBS}" = "x" || test "x${have_decklink}" = "xlocal" ],[
 decklink_dlopen=yes
])
AM_CONDITIONAL([GEM_DECKLINK_APISTUB], [test "x${decklink_dlopen}" = "xyes"])
AS_IF([test "x${decklink_dlopen}" = "xyes" ],[
 AC_DEFINE(GEM_DECKLINK_USE_DLOPEN, 1, [Define to 1 to open DeckLinkAPI via dlopen()])
 AC_MSG_NOTICE([use dlopen() to open DeckLinkAPI])
])
AC_SUBST([GEM_DECKLINK_USE_DLOPEN])


# NDI-support
AC_ARG_WITH([ndi],
            AC_HELP_STRING([--with-ndi=</path/to/NDI/SDK>],
               [enable NDI video capturing (use 'local' to use an embedded copy of the SDK)]))

have_ndi="no"
AS_IF([test "x$with_ndi" != "xno"], [
  tmp_ndi_includes=""
  AS_IF([test "x${with_ndi}" = "xyes" ], [ with_ndi="local" ])
  AS_IF([test "x${with_ndi}" = "x" ], [ with_ndi="local" ])
  AS_IF([test "x${with_ndi}" = "xlocal" ], [ have_ndi="yes"], [
        AS_IF([test -d "${with_ndi}/include"],
              [tmp_ndi_includes="${tmp_ndi_includes} -I${with_ndi}/include"],
             )
  CPPFLAGS="$CPPFLAGS $tmp_ndi_includes"

  AC_CHECK_HEADER(Processing.NDI.Lib.h,
                  [   have_ndi="yes"
                  ],[ have_ndi="no"
                  ])

  CPPFLAGS="$tmp_ndi_cppflags_org"
])

  AS_IF([test "x$have_ndi" = "xyes"],[
    AC_DEFINE([HAVE_NDI], [1], [video capturing using NDI by NewTek])
    GEM_NDI_CXXFLAGS="${tmp_ndi_includes}"
    GEM_NDI_LIBS=""
  ])

  AC_MSG_CHECKING([for NDI])
  AC_MSG_RESULT([$have_ndi])
])

AM_CONDITIONAL(HAVE_NDI, [test "x$have_ndi" = "xyes"])
AM_CONDITIONAL([GEM_NDI_LOCAL], [test "x${with_ndi}" = "xlocal"])
AC_SUBST(GEM_NDI_CXXFLAGS)
AC_SUBST(GEM_NDI_LIBS)



# enabling "-freg-struct-return" (for FreeFrame)
# i wonder could we make this the default ???
AS_IF([test "x$enable_registerstruct" != "xno" ], [
GEM_CHECK_CXXFLAGS([-freg-struct-return],
		[enable_registerstruct="yes"], [enable_registerstruct="no"])

AS_IF([test "x$enable_registerstruct" = "xyes" ], [
 AC_MSG_RESULT([enabling "register struct return"
   allows Gem to make use of FreeFrame-effects without recompiling them
   might break things, if you are using several different compilers])
])])

AS_IF([test "x$WINDOWS" = "xyes"], [GEM_CHECK_CXXFLAGS([-mms-bitfields])])
AS_IF([test "x$WINDOWS" = "xyes"], [GEM_CHECK_CXXFLAGS([-fexceptions])])
AS_IF([test "x$KERN" = "xDarwin"], [
 GEM_CHECK_CXXFLAGS([-Os],,GEM_CHECK_CXXFLAGS([-O3]))
], [
# shall we built FAT-binaries ?
## this needs more thinking! running just the preprocessor does not like these flags
## for now we could live with just adding these manually with GEM_EXTRA_CXXFLAGS and GEM_EXTRA_LDFLAGS
dnl GEM_CHECK_CXXFLAGS([-arch i386 -arch ppc], GEM_CHECK_LDFLAGS([-arch i386 -arch ppc], [EXT="d_fat"]))
 GEM_CHECK_CXXFLAGS([-O3])
])

dnl this is only useful for debugging and might break things on older compilers
dnl GEM_CHECK_CXXFLAGS([-fno-builtin])

GEM_CHECK_CXXFLAGS([-falign-loops -falign-functions -falign-jumps -funroll-loops],,
        GEM_CHECK_CXXFLAGS([-falign-loops -funroll-loops])
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
# the user can override this by explicitly stating that they want the
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

GEM_CHECK_FAT(GEM_ARCH)
AC_MSG_RESULT($GEM_ARCH)

AS_IF([test "x${ARCH}" = "xi386" || test "x${ARCH}" = "xi686" || test "x${ARCH}" = "xx86_64"], [
 AS_IF([test "x$enable_altivec" != "xyes"], [enable_altivec="no"])
])

AS_IF([test "x${ARCH}" = "xPower Macintosh" || test "x${ARCH}" = "xpowerpc"], [
 AS_IF([test "x$enable_mmx"  != "xyes"], [enable_mmx="no"])
 AS_IF([test "x$enable_sse2" != "xyes"], [enable_sse2="no"])
])

AS_IF([test "x$enable_mmx" != "xno" ],[GEM_CHECK_CXXFLAGS([-mmmx])])
AS_IF([test "x$enable_sse2" = "xyes" ],[GEM_CHECK_CXXFLAGS([-msse2])])
AS_IF([test "x$enable_altivec" != "xno"], [GEM_CHECK_CXXFLAGS([-faltivec])])
AS_IF([test "x$enable_powerpc" = "xyes"], [GEM_CHECK_CXXFLAGS([-mpowerpc-gfxopt])])

# on OSX we might come across "pascal strings" containing "\p"...
AS_IF([test "x$enable_pascalstrings" != "xno"], [GEM_CHECK_CXXFLAGS([-fpascal-strings])])

AC_ARG_WITH([defaultwindow],
             AS_HELP_STRING([--with-defaultwindow=<>],[specify default window backend (default: auto)]))

AS_CASE(["$with_defaultwindow"],
 [gem*], [GEM_DEFAULT_WINDOW="$with_defaultwindow"],
         [GEM_DEFAULT_WINDOW=""])

AS_IF([test "x${GEM_DEFAULT_WINDOW}" = "x"], [
 AS_IF([test "x${have_glfw3}" = "xyes"], [GEM_DEFAULT_WINDOW="gemglfw3window"],
       [test "x$WINDOWS" = "xyes"], [GEM_DEFAULT_WINDOW="gemw32window"],
       [test "x$DARWIN" = "xyes"], [GEM_DEFAULT_WINDOW="gemcocoawindow"],
       [test "x$no_glx" != "xyes"], [GEM_DEFAULT_WINDOW="gemglxwindow"],
       [test "x$no_glut" != "xyes"], [GEM_DEFAULT_WINDOW="gemglutwindow"]
       )
])
AC_SUBST(GEM_DEFAULT_WINDOW)
AC_CONFIG_FILES([abstractions/gemdefaultwindow.pd])

## allow us to override the build date string
AC_ARG_WITH([build-date],
            AC_HELP_STRING([--with-build-date=<str>],
                           [alternative date string]))
AC_MSG_CHECKING([build-date])
AS_IF([test "x$with_build_date"  = "xyes" ],[with_build_date=""])
AS_IF([test "x$with_build_date"  = "xno"  ],[with_build_date=""])
AS_IF([test "x$with_build_date" != "x"    ],
      [AC_DEFINE_UNQUOTED([BUILD_DATE], ["$with_build_date"], [user-defined build-date string])])
## urgh, brackets need to be escaped with @<:@ and @:>@...
AS_IF([test "x$with_build_date" != "x"  ],[ with_build_date="@<:@default@:>@" ])
AC_MSG_RESULT([$with_build_date])


## dummy conditionals that are always "true" (ENABLED) or "false" (DISABLED)
enabled="yes"
AM_CONDITIONAL([ENABLED], test "x${enabled}" = "xyes")
AM_CONDITIONAL([DISABLED], test "x${enabled}" = "xno")


# damned: gcc on osX seems to report that it supports "-ip -ipo_obj" although they are only icc flags
dnl  GEM_CHECK_CXXFLAGS([-ip -ipo_obj])

dnl we want dos2unix...
dnl AC_CHECK_TOOL([DOS2UNIX], [dos2unix], [true])

AC_OUTPUT

dnl GEM compilation options:
dnl =======================================
dnl
dnl give some feedback about the configuration

AS_IF([test "x$have_ftgl" = "xyes" ], [have_font=FTGL])
AS_IF([test "x$have_ImageMagick" = "xyes"], [have_ImageMagick="yes (Magick++${magick7})"],
      [test "x$have_MagickCore" = "xyes"], [have_ImageMagick="yes (MagickCore${magick7})"])

AS_IF([test "x$have_quicktime_framework" = "x" ], [have_quicktime_framework="no"])
AS_IF([test "x$have_quicktime_framework" = "xyes"], [have_libquicktime="yes (framework)"])

AS_IF([test "x$have_avfoundation_framework" = "x"], [have_avfoundation_framework="no"])
AS_IF([test "x$have_avfoundation_framework" = "xyes"], [
 have_libavfoundation="yes (framework)"
 GEM_FRAMEWORK_AVFOUNDATION_LIBS="$GEM_FRAMEWORK_AVFOUNDATION_LIBS -framework Accelerate -framework CoreMedia"
])

AS_IF([test "x$have_imageio_framework" = "x"], [have_imageio_framework="no"])
AS_IF([test "x$have_imageio_framework" = "xyes"], [have_libimageio="yes (framework)"])

have_ieee1394=""
AS_IF([test "x$have_libdc1394_2" = "xyes"], [have_ieee1394="$have_ieee1394 libdc1394"])
AS_IF([test "x$have_ieee1394" = "x"], [have_ieee1394="no"], [have_ieee1394="yes (${have_ieee1394# })"])
AS_IF([test "x${have_v4l}" = "x"], [have_v4l="no"])
AS_IF([test "x${have_v4l2}" = "x"], [have_v4l2="no"])
AS_IF([test "x${have_ieee1394}" = "x"], [have_ieee1394="no"])
AS_IF([test "x${have_libdv}" = "x"], [have_libdv="no"])
AS_IF([test "x${have_libdv}" != "xno"], [
  AS_IF([test "x${have_libiec61883}" != "xyes"], [have_libdv="no (missing libiec61883)"])
  AS_IF([test "x${have_libraw1394}" != "xyes"], [have_libdv="no (missing libraw1394)"])
])
AS_IF([test "x${have_unicap}" = "x"], [have_unicap="no"])
AS_IF([test "x${have_pipewire}" = "x"], [have_pipewire="no"])
AS_IF([test "x${have_vfw32}" = "x"], [have_vfw32="no"])
AS_IF([test "x${have_stb}" = "xyes"], [have_stb="YES (library)"], [have_stb="YES (local)"])

AS_IF([test "x${have_decklink}" = "xlocal"], [have_DeckLink="YES (local)"])
AS_IF([test "x${with_ndi}" = "xlocal"], [have_ndi="YES (local)"])
# TODO: have_ds

AC_MSG_RESULT([
Result:
  Target                 : Gem.${EXT}
  Objects                : ${GEM_TARGETS}
  default window         : ${GEM_DEFAULT_WINDOW}

Configuration:
  Compiler               : ${CXX}
  CXXFLAGS               : ${CXXFLAGS} ${X_CFLAGS}
                         : ${GEM_CHECK_LIB_CFLAGS}
  DEFINES                : ${DEFINES}

  LIBS                   : ${LIBS}
                         : ${GEM_CHECK_LIB_LIBS}
  LDFLAGS                : ${LDFLAGS}
                         : ${X_LIBS}

  Install path           : ${prefix}

 RTE (${GEM_RTE}):
  external-extension     : ${GEM_RTE_EXTENSION}
  CPPFLAGS               : ${GEM_RTE_CPPFLAGS}
  CFLAGS                 : ${GEM_RTE_CFLAGS}
  CXXFLAGS               : ${GEM_RTE_CXXFLAGS}
  LIBS                   : ${GEM_RTE_LIBS}

 used optional libraries:

  font-rendering         : ${have_font}
     default font        : ${GEM_DEFAULT_FONT}
])
AS_IF([test "$enable_plugins" != "no" ], [
 AC_MSG_RESULT([
PLUGINS
=======
  image-support
    use ImageIO          : ${have_imageio_framework}
    use JPEG             : ${have_jpeg}
    use ImageMagick      : ${have_ImageMagick}
    use QuickTime        : ${have_libquicktime}
    use SGI              : YES (local)
    use STB              : ${have_stb}
    use TIFF             : ${have_tiff}
  film-support
    use AVFoundation     : ${have_avfoundation_framework}
    use AVI              : ${WINDOWS}
    use aviplay          : ${have_avifile}
    use DirectShow       : ${WINDOWS}
    use FFMPEG           : ${have_ffmpeg}
    use gmerlin          : ${have_gmerlin_avdec}
    use mpeg             : ${have_mpeg}
    use mpeg-3           : ${have_libmpeg3}
    use QuickTime        : ${have_libquicktime}
  capture-support
    use AVfoundation     : ${have_avfoundation_framework}
    use DeckLink         : ${have_DeckLink}
    use DirectShow       : NO
    use DV               : ${have_libdv}
    use ieee1394         : ${have_ieee1394}
    use NDI              : ${have_ndi}
    use PipeWire         : ${have_pipewire}
    use Unicap           : ${have_unicap}
    use v4l              : ${have_v4l}
    use v4l2             : ${have_v4l2}
    use Video-for-WinDOS : ${have_vfw32}
    use VideoInput       : ${WINDOWS}
    use VLC              : ${have_libvlc}
    use VNC              : ${have_libvncclient}
  record-support
    use DeckLink         : ${have_DeckLink}
    use NDI              : ${have_ndi}
    use PNM              : YES (local)
    use QuickTime        : ${have_libquicktime}
    use v4l              : ${have_v4l}
    use v4l2             : ${have_v4l2}
  model-support
    use OBJ              : YES (local)
    use ASSIMP2          : ${have_assimp2}
    use ASSIMP3          : ${have_assimp3}
 ])],[
 AC_MSG_RESULT([
PLUGINS
=======
  NONE
 ])
])

AS_IF([test "x$have_pd" = "xno"], [
  AC_MSG_ERROR([Pd (header) is mandatory and seems to be missing!])
])

AS_IF([test "x$no_gl" = "xyes"], [
 AC_MSG_ERROR([GL (headers) not found! you need openGL!!!])
])

AS_IF([test "x$no_glu" = "xyes" && test "x$no_agl" = "xyes"], [
 AS_IF([test "x$with_GLU" != "xno"],[AC_MSG_ERROR([GLU (headers) not found! you need openGL Utility Library!!!])])
])

AC_MSG_RESULT([Now run make ...])
