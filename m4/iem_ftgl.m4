dnl FTGL support
dnl
dnl Copyright (C) 2011-2014 IOhannes m zmölnig


AC_DEFUN([IEM_CHECK_FTGL],
[
dnl PKG_CHECK_MODULES(PKG_FT2,freetype2)

GEM_CHECK_LIB([ftgl], [ftgl],, [main],,,[-lfreetype],[font support])

AS_IF([test "x${have_ftgl}" = "xyes"],[
 tmp_gem_check_ftgl_cppflags="$CPPFLAGS"
 CPPFLAGS="$CPPFLAGS $GEM_LIB_FTGL_CFLAGS"

 have_ftgl=no
 AC_CHECK_HEADERS([FTGL/ftgl.h FTFont.h], [have_ftgl=yes])

 CPPFLAGS="$tmp_gem_check_lib_cppflags"
 ])
AS_IF([test "x${have_ftgl}" != "xyes"],[
 GEM_LIB_FTGL_CFLAGS=""
 GEM_LIB_FTGL_LIBS=""
])
AM_CONDITIONAL([HAVE_LIB_FTGL], [test "x{have_ftgl}" = xyes ])
]) dnl IEM_CHECK_FTGL
