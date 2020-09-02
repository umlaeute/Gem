AC_PREREQ(2.60)
AC_INIT([videoNDI],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoNDI.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_FAT(GEM_ARCH)
GEM_CHECK_EXTERNAL

AC_LANG(C++)

AC_ARG_WITH([ndi],
            AC_HELP_STRING([--with-ndi=</path/to/NDI/SDK>],
               [enable NDI video capturing (use 'local' to use an embedded copy of the SDK)]))

have_ndi="no"
AS_IF([test "x$with_ndi" != "xno"], [
  tmp_ndi_includes=""
  AS_IF([test "x${with_ndi}" = "xyes" ], [ with_ndi="local" ])
  AS_IF([test "x${with_ndi}" = "x" ], [ with_ndi="local" ])
  AS_IF([test "x${with_ndi}" = "xlocal" ],
        [tmp_ndi_includes="${tmp_ndi_includes} -I${srcdir}/NDI"],
        AS_IF([test -d "${with_ndi}/include"],
              [tmp_ndi_includes="${tmp_ndi_includes} -I${with_ndi}/include"],
             ))

  CPPFLAGS="$CPPFLAGS $tmp_ndi_includes"

  AC_CHECK_HEADER(Processing.NDI.Lib.h,
                  [   have_ndi="yes"
                  ],[ have_ndi="no"
                  ])

  CPPFLAGS="$tmp_ndi_cppflags_org"

  AS_IF([test "x$have_ndi" = "xyes"],[
    AC_DEFINE([HAVE_NDI], [1], [video capturing using NDI by NewTek])
    GEM_NDI_CXXFLAGS="${tmp_ndi_includes}"
    GEM_NDI_LIBS=""
  ])

  AC_MSG_CHECKING([for NDI])
  AC_MSG_RESULT([$have_ndi])
])

AM_CONDITIONAL(HAVE_NDI, [test "x$have_ndi" = "xyes"])

AC_SUBST(GEM_NDI_CXXFLAGS)
AC_SUBST(GEM_NDI_LIBS)

AC_OUTPUT
