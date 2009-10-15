AC_PREREQ(2.57)
AC_INIT([pix_artoolkit],[0.1],[zmoelnig@iem.at])
AC_LANG(C++)



AC_SUBST(EXTENSION)
AC_ARG_WITH([extension], 
		AS_HELP_STRING([--with-extension=<ext>],[enforce a certain file-extension (e.g. pd_linux)]))
### weird configuration hacks
# the extension
if test "x$with_extension" != "x"; then
 EXTENSION=$with_extension
else
 EXTENSION=pd_`echo $host_os | sed -e '/.*/s/-.*//' -e 's/\[.].*//'`

 if test "x$KERN" != "xDarwin";
 then
  if test "x$host_os" = "x"
  then
 dnl just assuming that it is still linux (e.g. x86_64)
   EXTENSION="pd_linux"
  fi
 else
   EXTENSION="pd_darwin"
 fi
fi

## LDFLAGS
#check for "-shared -Wl,--export-dynamic"
#LDFLAGS+="-shared -Wl,--export-dynamic"
#check for "-bundle -undefined dynamic_lookup"


AC_CHECK_HEADERS([AR/ar.h])
AC_CHECK_LIB(AR, arInitCparam)


AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Make.config])
AC_OUTPUT
