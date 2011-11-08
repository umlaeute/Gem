#!/bin/sh

package=Gem

KERN=$(uname -s)

# debugging output to see which path is in effect
echo PATH: $PATH

IEM_AUTORECONF=$(which autoreconf)

IEM_AUTOHEADER=$(which autoheader)
IEM_AUTOMAKE=$(which automake)
IEM_ACLOCAL=$(which aclocal)
IEM_LIBTOOL=$(which libtool)
IEM_LIBTOOLIZE=$(which libtoolize)
IEM_AUTOCONF=$(which autoconf)

case "${KERN}" in
 MINGW*)
## on MinGW autoreconf is (still?) known to be somewhat broken
   IEM_AUTORECONF=""
   ;;
 *)
  ;;
esac

## debug printout to see which build scripts we are (or might be) using
echo "AUTORECONF: $IEM_AUTORECONF"
echo "AUTOHEADER: $IEM_AUTOHEADER"
echo "AUTOMAKE  : $IEM_AUTOMAKE"
echo "ACLOCAL   : $IEM_ACLOCAL"
echo "LIBTOOL   : $IEM_LIBTOOL"
echo "LIBTOOLIZE: $IEM_LIBTOOLIZE"
echo "AUTOCONF  : $IEM_AUTOCONF"


#check whether the system supports pushd/popd
if pushd . > /dev/null 2>&1
then
 popd > /dev/null 2>&1
else
## some shells (namely dash) don't support pushd/popd
## here we provide some dummies
pushd () {
 echo "ignoring pushd to $@"
}

popd () {
 echo "ignoring popd ..."
}
fi


autoconf_getsubdirs () {
 if [ -e configure.ac ]; then
 cat configure.ac | sed -e 's|#.*$||' | grep AC_CONFIG_SUBDIRS | \
	sed -e 's|^.*AC_CONFIG_SUBDIRS(\[\(.*\)\]).*$|\1|'
 fi
}

runit () {
echo "  $@"
$@
}

manual_autoreconf_doit () {
 echo faking autoreconf for $1
 pushd $1

  runit $IEM_ACLOCAL -I . -I $BASEDIR/m4 || exit 1

  runit $IEM_LIBTOOLIZE --automake -c || exit 1

  runit $IEM_AUTOCONF || exit 1

  if test -e configure.ac && grep AC_CONFIG_HEADER configure.ac > /dev/null 2>&1; then
   runit $IEM_AUTOHEADER --force || exit 1
  fi

  if [ -e Makefile.am ]; then
   runit $IEM_AUTOMAKE --add-missing -c || exit 1
  fi
 popd
}

manual_autoreconf () {
 echo faking autoreconf..
 BASEDIR=${0%/*}
 pushd $BASEDIR
 BASEDIR=$(pwd)
 popd

 if [ "x${SUBDIRS}" = "x" ]; then
  #SUBDIRS=autoconf_getsubdirs

  SUBDIRS="."
  SUBDIRS="${SUBDIRS} plugins/videoAVT plugins/videoHALCON plugins/videoPYLON"
  SUBDIRS="${SUBDIRS} extra extra/pix_artoolkit"
 fi


# check for all the needed helpers
 DIE=0
($IEM_AUTOCONF --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have autoconf installed to compile $package."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
        DIE=1
}

($IEM_AUTOMAKE --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}

($IEM_ACLOCAL --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have aclocal installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}

($IEM_LIBTOOL --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have libtool installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}
($IEM_LIBTOOLIZE --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have libtoolize installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}

if test "$DIE" -eq 1; then
        exit 1
fi

for s in ${SUBDIRS}; do
 manual_autoreconf_doit ${BASEDIR}/${s}
done
}


if test x$IEM_AUTORECONF != x; then
  echo running autoreconf
  $IEM_AUTORECONF --force --verbose --install 
else
  echo "not running autoreconf...falling back to"

  manual_autoreconf
fi
