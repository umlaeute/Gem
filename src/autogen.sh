#!/bin/sh

package=Gem


AUTORECONF=autoreconf

AUTOHEADER=autoheader
AUTOMAKE=automake
ACLOCAL=aclocal
LIBTOOL=libtool
LIBTOOLIZE=libtoolize
AUTOCONF=autoconf


KERN=$(uname -s)
case "${KERN}" in
 Darwin)
   PATH=/sw/bin:${PATH}
   ;;
 MINGW*)
   AUTORECONF=""
   ;;
 *)
  echo "kernel $KERN"
  ;;
esac


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
 if [ -e configure.in ]; then
 cat configure.in | sed -e 's|#.*$||' | grep AC_CONFIG_SUBDIRS | \
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

  SUBDIRS=$(autoconf_getsubdirs)

  runit $ACLOCAL -I $BASEDIR/m4 || exit 1
  runit $LIBTOOLIZE --automake -c || exit 1
  if [ -e Makefile.am ]; then
   runit $AUTOMAKE --add-missing -c || exit 1
  fi
  if test -e configure.ac && grep AC_CONFIG_HEADER configure.ac > /dev/null 2>&1; then
   runit $AUTOHEADER --force || exit 1
  fi

  for d in ${SUBDIRS}; do
    manual_autoreconf_doit ${d}
  done

  echo "  $AUTOCONF"
  $AUTOCONF || exit 1

 popd
}

manual_autoreconf () {
 echo faking autoreconf..
 BASEDIR=${0%/*}
 pushd $BASEDIR
 BASEDIR=$(pwd)
 popd

# check for all the needed helpers
 DIE=0
($AUTOCONF --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have autoconf installed to compile $package."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
        DIE=1
}

($AUTOMAKE --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}

($ACLOCAL --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have aclocal installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}

($LIBTOOL --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have libtool installed to compile $package."
        echo "Download the appropriate package for your system,"
        echo "or get the source from one of the GNU ftp sites"
        echo "listed in http://www.gnu.org/order/ftp.html"
        DIE=1
}
($LIBTOOLIZE --version) < /dev/null > /dev/null 2>&1 || {
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

manual_autoreconf_doit $BASEDIR
}


echo PATH: $PATH

if test x$AUTORECONF != x; then
  echo running autoreconf
  $AUTORECONF --force --verbose --install 
else
  echo not running autoreconf...
  manual_autoreconf
fi
