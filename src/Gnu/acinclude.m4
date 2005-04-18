dnl
dnl Check for a library in the system or local 
dnl arguments: 1: the librabry name without lib
dnl            2: the path to start searching for the local lib

dnl 
dnl Search a library in system and local
dnl AC_FIND_LIB (library, function, path[, force])

AC_DEFUN(AC_FIND_LIB,
   ac_lib_var=`echo $1['_']main | sed 'y%./+-%__p_%'`
     if test "$3"
     then
	testfun=$2
	dirs="$3"
     else
	testfun=main
	dirs="$2"
     fi

dnl next 2 lines had been dnl'ed. why?
     ac_save_LIBS=$LIBS
     gem_system_lib=no
     AC_CHECK_LIB($1,$testfun,gem_system_lib=yes)

dnl     ac_save_LIBS=$LIBS
dnl     AC_CHECK_LIB($1,$testfun)

dnl     if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = no" && \
dnl              test "${gem_system_lib-no}" != "yes"; then
dnl       AC_CHECK_LIB($1,$testfun,gem_system_lib=yes)
dnl     fi
     if eval "test \"`echo '$ac_cv_lib_'$ac_lib_var`\" = no" && \
              test "${gem_system_lib-no}" != "yes"; then

        AC_MSG_CHECKING("alternate -l$1 in $dirs")
        AC_CACHE_VAL(
          gem_cv_lib_$1,
dnl dnl       twice_lib=`dirname \`find $dirs -name "lib$1.a" | sed 1q\``
dnl          twice_lib=`find $dirs -name "lib$1.a" | sed 1q`
dnl	     if test "$twice_lib"
	tmp_lib=`find $dirs -name "lib$1.a"`
	  if test "$tmp_lib"
	  then
dnl	    twice_lib=`dirname $twice_lib`
	    for t in `ls -tr $tmp_lib`; do twice_lib=`dirname $t`; done

            dnl this time it should work
            save_LIBS=$LIBS
            LIBS="-l$1 $LIBS"
            save_LDFLAGS=$LDFLAGS
            LDFLAGS="$LDFLAGS -L$twice_lib";
            AC_TRY_LINK(,"main() { $testfun(); }",gem_cv_lib_$1=$twice_lib,
               if test $4; then AC_MSG_ERROR("library $1 not available - try makeauxlibs"); fi
            )
            LIBS=$save_LIBS
            LDFLAGS=$save_LDFLAGS
          fi
        )
dnl         echo ${gem_cv_lib_$1}

         if eval "test \"`echo '$''{'gem_cv_lib_$1'+set}'`\" = set"; then
          AC_MSG_WARN("Trying to use $1 library in ${gem_cv_lib_$1}");
dnl this might not be very portable, but at least it works for now
	  ac_tr_lib=`echo "HAVE_LIB$1" | sed -e 's/^a-zA-Z0-9_/_/g' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
	  AC_DEFINE_UNQUOTED($ac_tr_lib)
          LIBS="-l$1 $LIBS"
	  LDFLAGS="$LDFLAGS -L${gem_cv_lib_$1}"
         else
	  my_error_mess="Build $1 library somewhere";
          if test $4
	  then
	    AC_MSG_ERROR($my_error_mess);
	  else
	    AC_MSG_WARN($my_error_mess);
	  fi
         fi
     else
dnl i thought this would be handled by AC_CHECK_LIB
	ac_tr_lib=`echo "HAVE_LIB$1" | sed -e 's/^a-zA-Z0-9_/_/g' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/' -e 's/+/PLUS/g'`
	AC_DEFINE_UNQUOTED($ac_tr_lib)
        LIBS="-l$1 $LIBS"
     fi
)

AC_DEFUN(AC_FIND_HEADER,
dnl     AC_MSG_CHECKING(for $1 in $2 :: )
     dummy=""
dnl     tmp_path=`find $dirs -name "$1" | sed 1q`
     tmp_path="$2"
     tmp_file="$1"
     tmp_path=`find $tmp_path -name ${tmp_file##*/}`
     if test "$tmp_path"
     then
	found_path=""
	for t in `ls -tr $tmp_path`
	do
	  if test -d ${t%$tmp_file}
	  then 
	    found_path=${t%$tmp_file}
	  fi
	done
     	if test -z "$found_path"; then
       	  AC_MSG_WARN("$2 is no path to $1")
     	else
dnl       	 AC_MSG_RESULT($found_path)
	 dummy=`echo  $INCLUDES | grep -w -- "-I$found_path"`
	 if test -z "$dummy"
	 then
	   if test "$found_path" != "/usr/include/"
	   then
	     INCLUDES="-I$found_path $INCLUDES"
	   fi
	 fi
	 dummy="found"
	 break
	fi
     fi
     if [test "$dummy" = ""]; then
	if test $3; then
          AC_MSG_ERROR("can\'t find path to $1"); else
          AC_MSG_WARN("can\'t find path to $1"); fi
     else
	ac_hdr=`echo "HAVE_$1" | sed -e 's/^a-zA-Z0-9_./_/g' -e 's/\./_/g' -e 's/\//_/g' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/' -e 's/+/PLUS/g'`
	AC_DEFINE_UNQUOTED($ac_hdr)
     fi
)
