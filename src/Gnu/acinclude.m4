dnl
dnl Check for a library in the system or local 
dnl arguments: 1: the librabry name without lib 2: the path to start searching
dnl for the local lib

AC_DEFUN(AC_CHECK_LIB_TWICE,

     ac_lib_var=`echo $1['_']main | sed 'y%./+-%__p_%'`
     if [test "$3" = ""]
     then
	testfun=main
	dirs=$2
     else
	testfun=$2
	dirs=$3
     fi

     gem_system_lib=no
     AC_CHECK_LIB($1,main,gem_system_lib=yes)

     if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = no" && \
              test "${gem_system_lib-no}" != "yes"; then
       AC_CHECK_LIB($1,$testfun,gem_system_lib=yes)
     fi

     if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = no" && \
              test "${gem_system_lib-no}" != "yes"; then

        AC_MSG_CHECKING("alternate -l$1 in $dirs")
        AC_CACHE_VAL(
            gem_cv_lib_$1,
            twice_lib=`dirname \`find $dirs -name "lib$1.a" | sed 1q\``

            dnl this time it should work

            save_LIBS=$LIBS
            LIBS="-l$1 $LIBS"
            save_LDFLAGS=$LDFLAGS
            LDFLAGS="$LDFLAGS -L$twice_lib";

            AC_TRY_LINK(,"main() { $testfun(); }",gem_cv_lib_$1=$twice_lib,
               AC_MSG_ERROR("library $1 not available - try makeauxlibs")
            )
            LIBS=$save_LIBS
            LDFLAGS=$save_LDFLAGS
        )
         echo ${gem_cv_lib_$1}

         if eval "test \"`echo '$''{'gem_cv_lib_$1'+set}'`\" = set"; then
          AC_MSG_WARN("Trying to use $1 library in ${gem_cv_lib_$1}");
           LIBS="-l$1 $LIBS"                                                                                             LDFLAGS="$LDFLAGS -L${gem_cv_lib_$1}";                                                                   
	 else
          AC_MSG_WARN("Build $1 library somewhere");
         fi
     else
	LIBS="-l$1 $LIBS" 
     fi
)


AC_DEFUN(AC_PATH_INCLUDE,
     AC_MSG_CHECKING(for $1 path)
     dummy=""
     for dirs in $2
     do
     	tmp_path=""
     	tmp_path=`dirname \`find $dirs -name "$1" | sed 1q\``
     	if [test "$tmp_path" = ""]; then
       	  AC_MSG_WARN("$2 is no path to $1")
     	else
       	  AC_MSG_RESULT($tmp_path)
dnl       INCLUDES="-I`pwd`/$tmp_path $INCLUDES";
       	  INCLUDES="-I$tmp_path $INCLUDES";
	  dummy="found"
	  break
	fi
     done
     if [test "$dummy" = ""]; then
       AC_MSG_ERROR("can\'t find path to $1")
     fi
)

AC_DEFUN(AC_ADD_HAVELIB, HAVING_LIBS="-D$ac_tr_lib $HAVING_LIBS")
AC_DEFUN(AC_ADD_HAVEHEAD, HAVING_HEADERS="-D$ac_tr_head $HAVING_HEADERS")
