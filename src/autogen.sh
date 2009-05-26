#!/bin/sh

#-----------------------------------------------------------------------
# File: autogen.sh
# Description:
#   + wrapper for m4 black-magic
#-----------------------------------------------------------------------

# taken from Brian Jurish's readdir-external and simplfied for use with Gem

MY_ALDIRS="."
MY_ACDIRS="."

test -z "$ACLOCAL" && ACLOCAL=aclocal
test -z "$AUTOCONF" && AUTOCONF=autoconf

if test -n "$MY_ALDIRS"; then
 for d in $MY_ALDIRS ; do
    echo "(cd $d ; $ACLOCAL)"
    (cd $d ; $ACLOCAL)
 done
fi

if test -n "$MY_ACDIRS"; then
 for d in $MY_ACDIRS ; do
    echo "(cd $d ; $AUTOCONF)"
    (cd $d ; $AUTOCONF)
 done
fi


echo "if everything above succeeded, you can now run './configure'"
echo "for options see './configure --help'"
