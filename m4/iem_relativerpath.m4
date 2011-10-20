dnl Copyright (C) 2011 IOhannes m zmölnig
dnl This file is free software; IOhannes m zmölnig
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

## useage: add the @RELATIVERPATH_FLAGS@ to the LDFLAGS of your external

## TODO: add other platforms
##       on OSX we have to add "-install_name @loader_path/$NAME" when linking the _library_
##	 on W32 this should no be needed at all (but is it?)

AC_DEFUN([IEM_RELATIVERPATH],
[
originrpath_org_CFLAGS=$CFLAGS
IEM_CHECK_CFLAGS([-Wl,-enable-new-dtags -Wl,-rpath,"\$ORIGIN"],
[have_originrpath="yes"], [have_originrpath="no"])
CFLAGS=$originrpath_org_CFLAGS
RELATIVERPATH_FLAGS=""
if test "x$have_originrpath" = "xyes"; then
  RELATIVERPATH_FLAGS="-Wl,-enable-new-dtags -Wl,-rpath,\"\\\$\$ORIGIN\""
fi


AC_SUBST([RELATIVERPATH_FLAGS])
])# IEM_RELATIVERPATH
