dnl FTGL support
dnl
dnl Copyright (C) 2011 IOhannes m zmölnig


AC_DEFUN([IEM_CHECK_FTGL],
[
dnl PKG_CHECK_MODULES(PKG_FT2,freetype2)


GEM_CHECK_LIB([ftgl], [ftgl], [main],,,[-lfreetype],[font support])

tmp_gem_check_ftgl_cflags="$CFLAGS"
tmp_gem_check_ftgl_cxxflags="$CXXFLAGS"

CFLAGS="$CFLAGS $GEM_LIB_FTGL_CFLAGS"
CXXFLAGS="$CXXFLAGS $GEM_LIB_FTGL_CFLAGS"
AC_CHECK_HEADERS([FTGL/ftgl.h])

CFLAGS="$tmp_gem_check_lib_cflags"
CXXFLAGS="$tmp_gem_check_lib_cxxflags"
]) dnl IEM_CHECK_FTGL
