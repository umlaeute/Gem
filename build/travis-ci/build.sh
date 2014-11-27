#!/bin/sh
# this script is intended to be run by Travis
# see https://travis-ci.org/umlaeute/Gem

## it's based on avilleret's work:
# see https://travis-ci.org/avilleret/Gem

SCRIPTDIR=${0%/*}
GEMDIR=$(readlink -f "${SCRIPTDIR}/../..")

debug() {
 echo "$@" 1>&2
 $@
}

cd "${SCRIPTDIR}"

case "$TRAVIS_OS_NAME" in
 linux)
  PDDIR=/usr/include/pd
  BUILDDIR="${TRAVIS_OS_NAME}-amd64"

  #sudo apt-get install autopoint dh-autoreconf libdv4-dev libiec61883-dev \
  #  libmagick++-dev libmagick++5 libmagickwand-dev libpopt-dev libquicktime-dev libquicktime2 libunicap2 libunicap2-dev \
  #  libftgl-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev \
  #  libdc1394-dev
  sudo apt-get update -qq
  sudo apt-get build-dep gem -qq
  sudo apt-get install libglfw-dev libglfw3-dev libsdl2-devs -qq

  # wget http://msp.ucsd.edu/Software/pd-0.46-2.src.tar.gz
  # tar -xvf pd-0.46-2.src.tar.gz
  # PDDIR=${SCRIPTDIR}/pd-0.46-2/src

  debug ${GEMDIR}/autogen.sh  || exit 1

  mkdir -p "${SCRIPTDIR}/${BUILDDIR}"
  cd "${SCRIPTDIR}/${BUILDDIR}"
  debug "${GEMDIR}"/configure --without-ftgl --with-pd="${PDDIR}" && make
 ;;
 osx)
  PDDIR=/usr/include/pd
  BUILDDIR="${TRAVIS_OS_NAME}-amd64"

  brew update
  brew install pkg-config gettext
  brew install fribidi --universal
  brew link gettext --force
  brew install imagemagick ftgl
  brew install sdl homebrew/versions/glfw2 homebrew/versions/glfw3

  wget http://msp.ucsd.edu/Software/pd-0.46-2-64bit.mac.tar.gz
  tar -xf pd-0.46-2-64bit.mac.tar.gz

  wget http://msp.ucsd.edu/Software/pd-0.46-2.mac.tar.gz
  tar -xf pd-0.46-2.mac.tar.gz

  ${GEMDIR}/autogen.sh  || exit 1

  # 64bit build
  BUILDDIR="${TRAVIS_OS_NAME}-amd64"
  PDDIR=${SCRIPTDIR}/Pd-0.46-2-64bit.app/Contents/Resources/
  mkdir -p "${SCRIPTDIR}/${BUILDDIR}"
  cd "${SCRIPTDIR}/${BUILDDIR}"
  "${GEMDIR}/configure" --with-pd=${PDDIR}  --without-ftgl --without-QuickTime-framework --without-Carbon-framework && make
  EXIT64=$?

  # 32bit build
  BUILDDIR="${TRAVIS_OS_NAME}-i386"
  PDDIR=${SCRIPTDIR}/Pd-0.46-2.app/Contents/Resources/
  mkdir -p "${SCRIPTDIR}/${BUILDDIR}"
  cd "${SCRIPTDIR}/${BUILDDIR}"
  "${SCRIPTDIR}/configure" --with-pd=${PDDIR}  --enable-fat-binary=i386 --without-ftgl --without-QuickTime-framework --without-Carbon-framework && make
  EXIT32=$?

  exit $((EXIT64+EXIT32))
 ;;
esac


