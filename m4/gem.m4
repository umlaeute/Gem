dnl Copyright (C) 2005 IOhannes m zmölnig
dnl This file is free software; IOhannes m zmölnig
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.
# GEM_ARG_WITH(WITHARG, [TEXT], [FORCE])
# enables the "--with-WITHARG" flag; if FORCE is non-empty and no value is
# set by the user, the default value "yes" is assigned to with_WITHARG
# --------------------------------------------------------------
AC_DEFUN([GEM_ARG_WITH],
[
  AC_ARG_WITH([$1],
             AC_HELP_STRING([--without-$1], [disable $1-lib ($2)]),,[
                AS_IF([ test "x$3" != "x" ], [ with_$1="yes" ])
           ])
  AS_IF([ test "x${with_$1}" = "x" ], [ with_$1="${with_ALL}" ])

])# GEM_ARG_WITH
# inverse of GEM_ARG_WITH
AC_DEFUN([GEM_ARG_WITHOUT],
[AC_ARG_WITH([$1],
             AC_HELP_STRING([--with-$1], [enable $1-lib ($2)]),,[
                AS_IF([ test "x$3" = "xforce" ], [ with_$1="no" ])
           ])
])# GEM_ARG_WITHOUT

# same as GEM_ARG_WITH but with "enable"
AC_DEFUN([GEM_ARG_ENABLE],
[AC_ARG_ENABLE([$1],
               AC_HELP_STRING([--disable-$1], [disable $1 ($2)]),
               ,
               [enable_$1="yes"])
])# GEM_ARG_ENABLE

# inverse of GEM_ARG_ENABLE
AC_DEFUN([GEM_ARG_DISABLE],
[AC_ARG_ENABLE([$1],
               AC_HELP_STRING([--enable-$1], [enable $1 ($2)]),
               ,
               [enable_$1="no"])
])# GEM_ARG_DISABLE


AC_DEFUN([GEM_TARGET], [
define([NAME],[translit([$1],[abcdefghijklmnopqrstuvwxyz./+-],
                             [ABCDEFGHIJKLMNOPQRSTUVWXYZ____])])
AC_CONFIG_FILES([src/$1/Makefile])

AC_ARG_ENABLE([$1],
             AC_HELP_STRING([--disable-$1], [disable $1-objects]),
             [
                AS_IF([ test "x$enableval" != "xno" ], [
                  AC_MSG_RESULT([building Gem with $1-objects])
                  target_[]NAME="yes"
                ], [
                   AC_MSG_RESULT([building Gem without $1-objects])
                   target_[]NAME="no"
                ])
             ],
             [
                  AC_MSG_RESULT([building Gem with $1-objects])
                  target_[]NAME="yes"
             ])
AM_CONDITIONAL(TARGET_[]NAME, [test "x$target_[]NAME" != "xno"])
undefine([NAME])
])# GEM_TARGET

AC_DEFUN([GEM_TARGET_DISABLED], [
define([NAME],[translit([$1],[abcdefghijklmnopqrstuvwxyz./+-],
                             [ABCDEFGHIJKLMNOPQRSTUVWXYZ____])])

AC_CONFIG_FILES([src/$1/Makefile])
AC_ARG_ENABLE([$1],
             AC_HELP_STRING([--enable-$1], [enable $1-objects]),
             [
                AS_IF([ test "x$enableval" != "xyes" ], [
                   AC_MSG_RESULT([building Gem without $1-objects])
                   target_[]NAME="no"
                ],[
                  AC_MSG_RESULT([building Gem with $1-objects])
                  target_[]NAME="yes"
                ])
             ],
             [
                   AC_MSG_RESULT([building Gem without $1-objects])
                   target_[]NAME="no"
             ])
AM_CONDITIONAL(TARGET_[]NAME, [test "x$target_[]NAME" != "xno"])
undefine([NAME])
])# GEM_TARGET_DISABLED


# GEM_CHECK_LIB(NAME, LIBRARY, HEADERS, FUNCTION, [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND], [ADDITIONAL_LIBS], [HELP-TEXT], [DEFAULT-WITH_VALUE])
#
## this automatically adds the option "--without-NAME" to disable the checking for the library
##
## per default, this tries to use pkg-config or NAME-config to determine build flags.
## if this fails, LIBRARY (containing FUNCTION) and HEADERS are checked for explicitely
## both LIBRARY and *all* HEADERS must be present to succeed.
## HEADERS may be empty
##
## additionally this gives the options "--with-NAME-includes" and "--with-NAME-libs",
## to manually add searchpaths for the include-files and libraries
## the only check performed on these paths is whether they are real paths.
## using the ...-includes or ...-libs flags, overrides PKG_..._CFLAGS (resp PKG_..._LIBS)
AC_DEFUN([GEM_CHECK_LIB],
[
 define([Name],[translit([$1],[./-+], [____])])
 define([NAME],[translit([$1],[abcdefghijklmnopqrstuvwxyz./+-],
                              [ABCDEFGHIJKLMNOPQRSTUVWXYZ____])])
AC_SUBST(GEM_LIB_[]NAME[]_CFLAGS)
AC_SUBST(GEM_LIB_[]NAME[]_LIBS)

AC_ARG_WITH([Name],
             AC_HELP_STRING([--without-[]Name], [disable Name ($8)]))
AC_ARG_WITH([]Name-CFLAGS,
             AC_HELP_STRING([--with-[]Name-CFLAGS="-I/path/to/[]Name/include/"], [compiler flags for Name]))
AC_ARG_WITH([]Name-LIBS,
             AC_HELP_STRING([--with-[]Name-LIBS="-L/path/to/[]Name/lib/ -l$2"], [linker flags for Name]))

  AS_IF([ test "x$with_[]Name" = "x" ], [ with_[]Name="$9" ])
  AS_IF([ test "x$with_[]Name" = "x" ], [ with_[]Name="$with_ALL" ])

tmp_gem_check_lib_cppflags="$CPPFLAGS"
tmp_gem_check_lib_cflags="$CFLAGS"
tmp_gem_check_lib_cxxflags="$CXXFLAGS"
tmp_gem_check_lib_ldflags="$LDFLAGS"
tmp_gem_check_lib_libs="$LIBS"

AS_IF([ test x$with_[]Name = "xno" ],[
  have_[]Name="no (forced)"
],[
  have_[]Name="no (needs check)"


## first check for conflicting flags:
## - only one (1!) of _CFLAGS, _cflags and _includes must be present
  AS_IF([test "x${with_[]Name[]_CFLAGS}${with_[]Name[]_cflags}${with_[]Name[]_includes}" != "x" ],[
      AS_IF([test "x${with_[]Name[]_CFLAGS}" != "x" -a "x${with_[]Name[]_cflags}${with_[]Name[]_includes}" != "x" ],[
             AC_MSG_ERROR([you_ can only use one (1!) of --with-[]Name[]-CFLAGS (preferred), --with-[]Name[]-cflags and --with-[]Name[]-includes])
            ],[
	    AS_IF([test "x${with_[]Name[]_cflags}" != "x" -a "x${with_[]Name[]_includes}" != "x" ],[
                   AC_MSG_ERROR([you can only use one (1!) of --with-[]Name[]-CFLAGS (preferred), --with-[]Name[]-cflags and --with-[]Name[]-includes])
                  ])
            ])
      ])
## - only one (1!) of _LIBS and _libs must be present
    AS_IF([test "x${with_[]Name[]_LIBS}" != "x" -a "x${with_[]Name[]_libs}" != "x" ],[
           AC_MSG_ERROR([you can only use one (1!) of --with-[]Name[]-LIBS (preferred) and --with-[]Name[]-libs])
          ])

## then check for fallbacks
### - _CFLAGS defaults to _cflags defaults to _includes
  AS_IF([test "x${with_[]Name[]_CFLAGS}" = "x" ],[
    AS_IF([test "x${with_[]Name[]_cflags}" = "x"],[
           with_[]Name[]_CFLAGS=${with_[]Name[]_includes}
          ],[
           with_[]Name[]_CFLAGS=${with_[]Name[]_cflags}
          ])
        ])
### - _LIBS defaults to _libs
  AS_IF([test "x${with_[]Name[]_LIBS}" = "x" ],[
         with_[]Name[]_LIBS=${with_[]Name[]_libs}
        ])

## check for directories (so we need to prefix -I or -L)
  tmp_gem_check_lib_extracflags=""
  tmp_gem_check_lib_extralibs=""
  AS_IF([test -d "$with_[]Name[]_CFLAGS" ],
        [tmp_gem_check_lib_extracflags="-I$with_[]Name[]_CFLAGS"],
        [tmp_gem_check_lib_extracflags="$with_[]Name[]_CFLAGS"])
  AS_IF([test -d "$with_[]Name[]_LIBS" ],
        [tmp_gem_check_lib_extralibs="-L$with_[]Name[]_LIBS"],
        [tmp_gem_check_lib_extralibs="$with_[]Name[]_LIBS"])

  AS_IF([test "x$tmp_gem_check_lib_extracflags" != "x" ],[
    CFLAGS="$tmp_gem_check_lib_extracflags $CFLAGS"
    CPPFLAGS="$tmp_gem_check_lib_extracflags $CPPFLAGS"
    CXXFLAGS="$tmp_gem_check_lib_extracflags $CXXFLAGS"
    PKG_[]NAME[]_CFLAGS="$tmp_gem_check_lib_extracflags"
    ])

  AS_IF([test "x$tmp_gem_check_lib_extralibs" != "x" ],[
    LIBS="$tmp_gem_check_lib_extralibs $LIBS"
    PKG_[]NAME[]_LIBS="$tmp_gem_check_lib_extralibs"
    ])

  AS_LITERAL_IF([$2],
              [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$2_$4])],
              [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$2''_$4])])dnl

## unset ac_Lib is possible
  (unset ac_Lib) >/dev/null 2>&1 && unset ac_Lib

## 1st we check, whether pkg-config knows something about this package
dnl  PKG_CHECK_MODULES(AS_TR_CPP(PKG_$1), $1,AS_VAR_SET(acLib)yes, AC_CHECK_LIB([$2],[$4],,,[$7]))
  PKG_CHECK_MODULES(AS_TR_CPP(PKG_$1), $1,AS_VAR_SET([ac_Lib], [yes]),:)

  AS_IF([test "x$ac_Lib" != "xyes" ],[
## pkg-config has failed
## check whether there is a ${1}-config lying around
   AC_MSG_CHECKING([for $1-config])
   gem_check_lib_pkgconfig_[]NAME=""
   AS_IF([ test "x$1" != "x" ],[
    gem_check_lib_pkgconfig_[]NAME="$1"-config
    AS_IF([ which -- "$gem_check_lib_pkgconfig_[]NAME" >/dev/null 2>&1 ],[
     gem_check_lib_pkgconfig_[]NAME=$(which "$1"-config)
     AC_MSG_RESULT([yes])
    ],[
     gem_check_lib_pkgconfig_[]NAME=""
     AC_MSG_RESULT([no])
    ])
   ])

   AS_IF([ test "x$gem_check_lib_pkgconfig_[]NAME" != "x" ],[
## fabulous, we have ${1}-config
   AS_VAR_SET([ac_Lib], [yes])
## lets see what it reveals

## if PKG_<name>_CFLAGS is undefined, we try to get it from ${1}-config
    AS_IF([ test "x$PKG_[]NAME[]_CFLAGS" = "x" ], [
      AS_IF([ $gem_check_lib_pkgconfig_[]NAME --cflags >/dev/null 2>&1 ], [
        PKG_[]NAME[]_CFLAGS=$(${gem_check_lib_pkgconfig_[]NAME} --cflags)
      ])
    ])
 
## if PKG_<name>_LIBS is undefined, we try to get it from ${1}-config
## we first try to get it via "--plugin-libs" (this is almost certainly what we want)
## if that fails we try to get it via  "--libs"
    AS_IF([ test "x$PKG_[]NAME[]_LIBS" = "x" ], [
      AS_IF([ $gem_check_lib_pkgconfig_[]NAME --plugin-libs >/dev/null 2>&1 ], [
        PKG_[]NAME[]_LIBS=$(${gem_check_lib_pkgconfig_[]NAME} --plugin-libs)
      ],[
       AS_IF([ $gem_check_lib_pkgconfig_[]NAME --libs >/dev/null 2>&1 ], [
        PKG_[]NAME[]_LIBS=$(${gem_check_lib_pkgconfig_[]NAME} --libs)
       ])
      ])
    ])
   ])

## if we still don't know about the libs, we finally fall back to AC_CHECK_LIB / AC_CHECK_HEADERS
   AS_IF([ test "x$ac_Lib" != "xyes" ],[
     AS_VAR_SET([ac_Lib], [yes])
     AS_IF([ test "x${PKG_[]NAME[]_LIBS}" = "x" ],[
      AS_IF([ test "x$2" != "x" ], [
       AC_CHECK_LIB([$2],[$4],PKG_[]NAME[]_LIBS="-l$2",AS_VAR_SET([ac_Lib], [no]),[$7])
      ])
     ])
     AS_IF([ test "x$3" != "x" && test "x${ac_Lib}" != "xno" ],[
      AC_CHECK_HEADERS([$3],,AS_VAR_SET([ac_Lib], [no]))
     ])
   ])
  ])

  AS_IF([test "x$ac_Lib" = "xyes"],
   [
    AS_IF([ test "x$1" != "x" ], [
     AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIB$1),[1], [$8])
    ])
    AS_IF([ test "x$2" != "x" ], [
     AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIB$2),[1], [$8])
    ])
    AS_IF([ test "x$4" != "x" ], [
     AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_$4),[1], [Define to 1 if you have the `$4' function.])
    ])
    GEM_LIB_[]NAME[]_CFLAGS=${PKG_[]NAME[]_CFLAGS}
    GEM_LIB_[]NAME[]_LIBS=${PKG_[]NAME[]_LIBS}
    GEM_CHECK_LIB_CFLAGS="${GEM_LIB_[]NAME[]_CFLAGS} ${GEM_CHECK_LIB_CFLAGS}"
    GEM_CHECK_LIB_LIBS="${GEM_LIB_[]NAME[]_LIBS} ${GEM_CHECK_LIB_LIBS}"

    CPPFLAGS="${PKG_[]NAME[]_CFLAGS} ${CPPFLAGS}"
    LDFLAGS="${PKG_[]NAME[]_LIBS} ${LDFLAGS}"

    have_[]Name="yes"
dnl turn of further checking for this package
    with_[]Name="no"
    [$5]
   ],[
    have_[]Name="no"
    [$6]
   ])
   AS_VAR_POPDEF([ac_Lib])dnl

   AS_IF([ test "x$2" != "x" ], [
    AC_CHECK_LIB([$2],[$4],,,[$7])
   ])
   AS_IF([ test "x$3" != "x" ], [
    AC_CHECK_HEADERS([$3])
   ])
])

AM_CONDITIONAL(HAVE_LIB_[]NAME, [test "x${have_[]Name}" = "xyes"])
AS_IF([test "x${have_[]Name}" = "xyes" ],
      [AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_GEM_LIB_$1),[1], [$8])],[])
# restore original flags
CPPFLAGS="$tmp_gem_check_lib_cppflags"
CFLAGS="$tmp_gem_check_lib_cflags"
CXXFLAGS="$tmp_gem_check_lib_cxxflags"
LDFLAGS="$tmp_gem_check_lib_ldflags"
LIBS="$tmp_gem_check_lib_libs"


undefine([Name])
undefine([NAME])
])# GEM_CHECK_LIB

# GEM_CHECK_CXXFLAGS(ADDITIONAL-CXXFLAGS, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
#
# checks whether the $(CXX) compiler accepts the ADDITIONAL-CXXFLAGS
# if so, they are added to the CXXFLAGS
AC_DEFUN([GEM_CHECK_CXXFLAGS],
[
  AC_MSG_CHECKING([whether compiler accepts "$1"])
cat > conftest.c++ << EOF
int main(){
  return 0;
}
EOF
AS_IF([ $CXX $CPPFLAGS $CXXFLAGS -o conftest.o conftest.c++ [$1] > /dev/null 2>&1 ],[
  AC_MSG_RESULT([yes])
  CXXFLAGS="${CXXFLAGS} [$1]"
  [$2]
],[
  AC_MSG_RESULT([no])
  [$3]
])
])# GEM_CHECK_CXXFLAGS

# GEM_CHECK_FRAMEWORK(FRAMEWORK, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
#
#
AC_DEFUN([GEM_CHECK_FRAMEWORK],
[
  define([Name],[translit([$1],[./-+], [____])])
  define([NAME],[translit([$1],[abcdefghijklmnopqrstuvwxyz./+-],
                              [ABCDEFGHIJKLMNOPQRSTUVWXYZ____])])
  AC_SUBST(GEM_FRAMEWORK_[]NAME[]_CFLAGS)
  AC_SUBST(GEM_FRAMEWORK_[]NAME[]_LIBS)

  AC_ARG_WITH([Name]-framework,
             AC_HELP_STRING([--without-[]Name[]-framework], [disable Name-framework]))
  AC_ARG_WITH([]Name[]-framework-CFLAGS,
             AC_HELP_STRING([--with-[]Name[]-framework-CFLAGS="-I/path/to/[]Name/include/"], [compiler flags for Name-framework]))
  AC_ARG_WITH([]Name[]-framework-LIBS,
             AC_HELP_STRING([--with-[]Name[]-framework-LIBS="-L/path/to/[]Name/lib/ -l$2"], [linker flags for Name-framework]))
  AS_IF([ test "x$with_[]Name[]_framework" = "x" ], [ with_[]Name="$with_ALL" ])


  AC_MSG_CHECKING([for $1-framework])

AS_IF([ test x$with_[]Name[]_framework = "xno" ],[
  have_[]Name="no (forced)"
],[
 AS_IF([ test "x${with_[]Name[]_framework_CFLAGS}${with_[]Name[]_framework_LIBS}" != "x" ], [
## manually forced: yes
    GEM_FRAMEWORK_[]NAME[]_CFLAGS=${with_[]Name[]_framework_CFLAGS}
    GEM_FRAMEWORK_[]NAME[]_LIBS=${with_[]Name[]_framework_LIBS}
    have_[]Name="yes"
 ],[
  have_[]Name="no (needs check)"

  gem_check_ldflags_org="${LDFLAGS}"
  LDFLAGS="-framework [$1] ${LDFLAGS}"

  AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])], [gem_check_ldflags_success="yes"],[gem_check_ldflags_success="no"])

  AS_IF([ test "x${gem_check_ldflags_success}" = "xyes" ], [
    have_[]Name="yes"
    GEM_FRAMEWORK_[]NAME[]_CFLAGS=""
    GEM_FRAMEWORK_[]NAME[]_LIBS="-framework [$1]"
    [$2]
  ],[
    have_[]Name="no"
    [$3]
  ])
  LDFLAGS="${gem_check_ldflags_org}"
 ])
])
  AS_IF([ test "x${have_[]Name}" = "xyes" ],[
    AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_$1), [1], [$1 framework])
    AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_GEM_FRAMEWORK_$1), [1], [$1 framework])
  ])
  AC_MSG_RESULT([${have_[]Name}])
  AM_CONDITIONAL(HAVE_FRAMEWORK_[]NAME, [test "x${have_[]Name}" = "xyes"])
undefine([NAME])
undefine([Name])
])# GEM_CHECK_FRAMEWORK

# GEM_CHECK_LDFLAGS(ADDITIONAL-LDFLAGS, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
#
# checks whether the $(LD) linker accepts the ADDITIONAL-LDFLAGS
# if so, they are added to the LDFLAGS
AC_DEFUN([GEM_CHECK_LDFLAGS],
[
  AC_MSG_CHECKING([whether linker accepts "$1"])
  gem_check_ldflags_org="${LDFLAGS}"
  LDFLAGS="$1 ${LDFLAGS}"

  AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])], [gem_check_ldflags_success="yes"],[gem_check_ldflags_success="no"])

  AS_IF([ test "x$gem_check_ldflags_success" = "xyes" ],[
    AC_MSG_RESULT([yes])
    [$2]
  ],[
    AC_MSG_RESULT([no])
    LDFLAGS="$gem_check_ldflags_org"
    [$3]
  ])
])# GEM_CHECK_LDFLAGS


AC_DEFUN([GEM_CHECK_FAT],
[
AC_ARG_ENABLE(fat-binary,
       [  --enable-fat-binary=ARCHS
                          build an Apple Multi Architecture Binary (MAB);
                          ARCHS is a comma-delimited list of architectures for
                          which to build; if ARCHS is omitted, then the package
                          will be built for all architectures supported by the
                          platform (e.g. "ppc,i386" for MacOS/X and Darwin); 
                          if this option is disabled or omitted entirely, then
                          the package will be built only for the target 
                          platform; when building multiple architectures, 
			  dependency-tracking must be disabled],
       [fat_binary=$enableval], [fat_binary=no])
AS_IF([test "x$fat_binary" != "xno" ],[
    AC_MSG_CHECKING([target architectures])

    # Respect TARGET_ARCHS setting from environment if available.
    AS_IF([ test -z "$TARGET_ARCHS" ],[
       # Respect ARCH given to --enable-fat-binary if present.
     AS_IF([ test "x$fat_binary" != "xyes" ],[
       TARGET_ARCHS=$(echo "$fat_binary" | tr ',' ' ')
     ],[
       # Choose a default set of architectures based upon platform.
       TARGET_ARCHS="ppc i386"
     ])
    ])
    AC_MSG_RESULT([$TARGET_ARCHS])

   define([Name],[translit([$1],[./-], [___])])
   # /usr/lib/arch_tool -archify_list $TARGET_ARCHS
   []Name=""
   tmp_arch_count=0
   for archs in $TARGET_ARCHS 
   do
    []Name="$[]Name -arch $archs"
    tmp_arch_count=$((tmp_arch_count+1))
   done

   AS_IF([ test "$tmp_arch_count" -gt 1 ],[
     AS_IF([ test "x$enable_dependency_tracking" != xno ],[
     	AC_MSG_ERROR([when building for multiple architectures, you MUST turn off dependency-tracking])
     ])
   ])

   AS_IF([ test "x$[]Name" != "x" ],[
    tmp_arch_cflags="$CFLAGS"
    tmp_arch_cxxflags="$CXXFLAGS"
    GEM_CHECK_CXXFLAGS($[]Name,,[]Name="")
    []Name[]_CXXFLAGS+=$[]Name
    CFLAGS="$tmp_arch_cflags"
    CXXFLAGS="$tmp_arch_cxxflags"
   ])

   AS_IF([ test "x$[]Name" != "x" ],[
    tmp_arch_ldflags="$LDFLAGS"
    GEM_CHECK_LDFLAGS($[]Name,,[]Name="")
    []Name[]_LDFLAGS+=$[]Name
    LDFLAGS="$tmp_arch_ldflags"
   ])

   undefine([Name])
])
])# GEM_CHECK_FAT

# GEM_CHECK_RTE()
#
# checks for RTE (currently: Pd)
# if so, they are added to the LDFLAGS, CFLAGS and whatelse
AC_DEFUN([GEM_CHECK_RTE],
[
ARCH=$(uname -m)
KERN=$(uname -s)
IEM_OPERATING_SYSTEM

AC_SUBST(GEM_RTE_CFLAGS)
AC_SUBST(GEM_RTE_LIBS)
AC_SUBST(GEM_RTE)

AS_IF([ test "x${libdir}" = "x\${exec_prefix}/lib" ],[
 libdir='${exec_prefix}/lib/pd/extra'
])

tmp_rte_cppflags="$CPPFLAGS"
tmp_rte_cflags="$CFLAGS"
tmp_rte_cxxflags="$CXXFLAGS"
tmp_rte_ldflags="$LDFLAGS"
tmp_rte_libs="$LIBS"

GEM_RTE_CFLAGS="-DPD"
GEM_RTE_LIBS=""
GEM_RTE="Pure Data"

AC_ARG_WITH([pd], 
	        AS_HELP_STRING([--with-pd=<path/to/pd>],[where to find pd-binary (./bin/pd.exe) and pd-sources]))

## some default paths
AS_IF([ test "x${with_pd}" = "x" ],[
 case $host_os in
 *darwin*)
    AS_IF([ test -d "/Applications/Pd-extended.app/Contents/Resources" ], [ with_pd="/Applications/Pd-extended.app/Contents/Resources" ])
    AS_IF([ test -d "/Applications/Pd.app/Contents/Resources" ], [ with_pd="/Applications/Pd.app/Contents/Resources" ])
 ;;
 *mingw* | *cygwin*)
    AS_IF([ test -d "${PROGRAMFILES}/Pd-extended" ], [ with_pd="${PROGRAMFILES}/Pd-extended" ])
    AS_IF([ test -d "${PROGRAMFILES}/pd" ], [ with_pd="${PROGRAMFILES}/pd" ])
 ;;
 esac
])

AS_IF([ test -d "$with_pd"  ],[
 if test -d "${with_pd}/src" ; then
   AC_LIB_APPENDTOVAR([GEM_RTE_CFLAGS],"-I${with_pd}/src")
 elif test -d "${with_pd}/include/pd" ; then
   AC_LIB_APPENDTOVAR([GEM_RTE_CFLAGS],"-I${with_pd}/include/pd")
 elif test -d "${with_pd}/include" ; then
   AC_LIB_APPENDTOVAR([GEM_RTE_CFLAGS],"-I${with_pd}/include")
 else
   AC_LIB_APPENDTOVAR([GEM_RTE_CFLAGS],"-I${with_pd}")
 fi
 AS_IF([ test -d "${with_pd}/bin"  ],[
   GEM_RTE_LIBS="${GEM_RTE_LIBS}${GEM_RTE_LIBS:+ }-L${with_pd}/bin"
 ],[
   GEM_RTE_LIBS="${GEM_RTE_LIBS}${GEM_RTE_LIBS:+ }-L${with_pd}"
 ])

 CPPFLAGS="$CPPFLAGS ${GEM_RTE_CFLAGS}"
 CFLAGS="$CFLAGS ${GEM_RTE_CFLAGS}"
 CXXFLAGS="$CXXFLAGS ${GEM_RTE_CFLAGS}"
 LIBS="$LIBS ${GEM_RTE_LIBS}"
])

AC_CHECK_LIB([:pd.dll], [nullfn], [have_pddll="yes"], [have_pddll="no"])
AS_IF([ test "x$have_pddll" = "xyes" ], [
 GEM_RTE_LIBS="${GEM_RTE_LIBS}${GEM_RTE_LIBS:+ }-Xlinker -l:pd.dll"
],[
 AC_CHECK_LIB([pd], [nullfn], [GEM_RTE_LIBS="${GEM_RTE_LIBS}${GEM_RTE_LIBS:+ }-lpd"])
])

AC_CHECK_HEADERS([m_pd.h], [have_pd="yes"], [have_pd="no"])

dnl LATER check why this doesn't use the --with-pd includes
dnl for now it will basically disable anything that needs s_stuff.h if it cannot be found in /usr[/local]/include
AC_CHECK_HEADERS([s_stuff.h], [], [],
[#ifdef HAVE_M_PD_H
# define PD
# include "m_pd.h"
#endif
])

### this should only be set if Pd has been found
# the extension
AC_ARG_WITH([extension], 
		AS_HELP_STRING([--with-extension=<ext>],[enforce a certain file-extension (e.g. pd_linux)]))
AS_IF([ test "x$with_extension" != "x" ],[
 EXT=$with_extension
],[
  case x$host_os in
   x*darwin*)
     EXT=pd_darwin
     ;;
   x*mingw* | x*cygwin*)
     EXT=dll
     ;;
   x)
     dnl just assuming that it is still linux (e.g. x86_64)
     EXT="pd_linux"
     ;;
   *)
     EXT=pd_$(echo $host_os | sed -e '/.*/s/-.*//' -e 's/\[.].*//')
     ;;
  esac
])
GEM_RTE_EXTENSION=$EXT
AC_SUBST(GEM_RTE_EXTENSION)

CPPFLAGS="$tmp_rte_cppflags"
CFLAGS="$tmp_rte_cflags"
CXXFLAGS="$tmp_rte_cxxflags"
LDFLAGS="$tmp_rte_ldflags"
LIBS="$tmp_rte_libs"
]) # GEM_CHECK_RTE


AC_DEFUN([GEM_CHECK_THREADS],
[
GEM_ARG_ENABLE([threads], [default: use threads])

AC_SUBST(GEM_THREADS_CFLAGS)
AC_SUBST(GEM_THREADS_LIBS)

AS_IF([ test "x$enable_threads" != "xno" ],[
 AX_PTHREAD

 AS_IF([test "x$ax_pthread_ok" = "xyes"], [
   AC_CHECK_TYPES([ptw32_handle_t], [], [], [
   #include <pthread.h>
   ])
 ])

 GEM_THREADS_CFLAGS=""
 AC_LIB_APPENDTOVAR([GEM_THREADS_CFLAGS], "${PTHREAD_CFLAGS}")
 GEM_THREADS_LIBS="${GEM_THREADS_LIBS}${GEM_THREADS_LIBS:+ }${PTHREAD_LIBS}"
])
])
