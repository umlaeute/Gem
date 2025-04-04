

#g++ -O6 -march=pentium -mtune=pentiumpro -I/home/zmoelnig/pylon//include -I/home/zmoelnig/pylon//include/cpp  ./ean13.cpp -I/home/zmoelnig/pylon//include -I/home/zmoelnig/pylon//include/cpp \
#          -L/usr/X11R6/lib -lXext -lX11 -lpthread -lm -ldl -L/home/zmoelnig/pylon//lib/x86-linux2.4-gcc40/ -lpyloncpp -lpylon -o ../bin/x86-linux2.4-gcc40/ean13

## CFLAGS: -I${PYLON_ROOT}/include
## LDFLAGS: -L${PYLON_ROOT}/lib -L${PYLON_ROOT}/lib64 -lpylonbase


AC_DEFUN([GEM_CHECK_PYLON],
[
AC_ARG_VAR([PYLON_ROOT], [root-directory where you installed PYLON (override this with '--with-pylon=${PYLON_ROOT}'])dnl
AC_ARG_WITH([pylon],
             AS_HELP_STRING([--with-pylon], [enable PYLON video capturing (overrides $PYLON_ROOT)]))

tmp_pylon_rpath=""
have_pylon="no"
if test "x$with_pylon" != "xno"; then
  if test -d "${with_pylon}" ; then
    PYLON_ROOT=${with_pylon}
  fi

  tmp_pylon_includes=""
  if test -d "${PYLON_ROOT}/include" ; then
   tmp_pylon_includes="${tmp_pylon_includes} -I${PYLON_ROOT}/include"
  fi

  for d in "${PYLON_ROOT}/lib" "${PYLON_ROOT}/lib64"
  do
    if test -d "${d}"; then
      tmp_pylon_rpath="${tmp_pylon_rpath} -R${d}"
      tmp_pylon_ldflags="${tmp_pylon_ldflags} -L${d}"
    fi
  done

  tmp_pylon_ldflags="-lpylonbase -lpylonutility ${tmp_pylon_ldflags}"

  tmp_pylon_cppflags_org="$CPPFLAGS"
  tmp_pylon_libs_org="$LIBS"

  CPPFLAGS="$CPPFLAGS $tmp_pylon_includes"
  LIBS="-lm"

  AC_CHECK_HEADER([pylon/PylonIncludes.h],
                  [
                    have_pylon="yes"
                  ],[
                    have_pylon="no"
                  ])
  if test "x$have_pylon" = "xyes"; then
   AC_CHECK_LIB(pylonbase, main, , [have_pylon="no"], ["${tmp_pylon_ldflags}"])
   AC_CHECK_LIB(pylonutility, main, , [have_pylon="no"], ["${tmp_pylon_ldflags}"])
  fi

  CPPFLAGS="$tmp_pylon_cppflags_org"
  LIBS="$tmp_pylon_libs_org"

  if test "x$have_pylon" = "xyes"; then
    AC_DEFINE([HAVE_PYLON], [1], [video capturing using Basler's PYLON])
    GEM_PYLON_CXXFLAGS="${tmp_pylon_includes}"
    GEM_PYLON_LIBS="${tmp_pylon_ldflags} ${tmp_pylon_rpath}"
  fi

  AC_MSG_CHECKING([for PYLON])
  AC_MSG_RESULT([$have_pylon])

fi

AM_CONDITIONAL(HAVE_PYLON, test x$have_pylon = xyes)

AC_SUBST(GEM_PYLON_CXXFLAGS)
AC_SUBST(GEM_PYLON_LIBS)
])
