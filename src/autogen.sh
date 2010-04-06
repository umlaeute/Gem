#!/bin/sh

#-----------------------------------------------------------------------
# File: autogen.sh
# Description:
#   + wrapper for m4 black-magic
#-----------------------------------------------------------------------

# taken from Brian Jurish's readdir-external and simplfied for use with Gem

MY_ALDIRS=". extra/pix_artoolkit"
MY_ACDIRS=". extra/pix_artoolkit"
MY_M4DIRS=`pwd`/m4

test -z "$ACLOCAL" && ACLOCAL=aclocal
test -z "$AUTOCONF" && AUTOCONF=autoconf
test -z "$AUTORECONF" && AUTORECONF=autoreconf

# check whether autoreconf is up and running
echo checking $AUTORECONF
if $AUTORECONF --version > /dev/null 2>&1; then
 :
else
 AUTORECONF=""
fi

if test -n "$AUTORECONF"; then
 $AUTORECONF --force --verbose --install -I $MY_M4DIRS
else

 if test -n "$MY_ALDIRS"; then
  for d in $MY_ALDIRS ; do
     echo "(cd $d ; $ACLOCAL -I $MY_M4DIRS)"
     (cd $d ; $ACLOCAL -I $MY_M4DIRS)
  done
 fi
 
 if test -n "$MY_ACDIRS"; then
  for d in $MY_ACDIRS ; do
     echo "(cd $d ; $AUTOCONF)"
     (cd $d ; $AUTOCONF)
  done
 fi
fi

echo "if everything above succeeded, you can now run './configure'"
echo "for options see './configure --help'"
