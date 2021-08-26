dnl try to figure out the target operating system and set some AM-macros accordingly 
dnl
dnl Copyright (C) 2011 IOhannes m zmölnig


AC_DEFUN([IEM_OPERATING_SYSTEM],
[
AC_CANONICAL_HOST

LINUX=no
ANDROID=no
MACOSX=no
IPHONEOS=no
BSD=no
WINDOWS=no
MINGW=no
CYGWIN=no
HURD=no
IRIX=no
EMSCRIPTEN=no

case $host_os in
*linux*)
	 LINUX=yes
  ;;
*darwin*)
	 MACOSX=yes
	;;
GNU/kFreeBSD)
   BSD=yes
	;;
*mingw*)
	WINDOWS=yes
	MINGW=yes
	;;
*cygwin*)
	WINDOWS=yes
	CYGWIN=yes
  ;;
GNU)
	 HURD=yes
  ;;
esac

AC_LANG_PUSH([C])
AC_MSG_CHECKING([for GNU triplet])
cc_triplet=$("${CC}" -dumpmachine 2>/dev/null || true)
AC_MSG_RESULT([${cc_triplet}])
AS_CASE([$cc_triplet],
  [*-emscripten], [EMSCRIPTEN=yes],
  [])
AC_LANG_POP([C])


AM_CONDITIONAL(LINUX, test x$LINUX = xyes)
AM_CONDITIONAL(ANDROID, test x$ANDROID = xyes)
AM_CONDITIONAL(MACOSX, test x$MACOSX = xyes)
AM_CONDITIONAL(IPHONEOS, test x$IPHONEOS = xyes)
AM_CONDITIONAL(BSD, test x$BSD = xyes)
AM_CONDITIONAL(WINDOWS, test x$WINDOWS = xyes)
AM_CONDITIONAL(CYGWIN, test x$CYGWIN = xyes)
AM_CONDITIONAL(MINGW, test x$MINGW = xyes)
AM_CONDITIONAL(HURD, test x$HURD = xyes)
AM_CONDITIONAL(IRIX, test x$IRIX = xyes)
AM_CONDITIONAL(EMSCRIPTEN, test x$EMSCRIPTEN = xyes)
]) dnl IEM_OPERATING_SYSTEM
