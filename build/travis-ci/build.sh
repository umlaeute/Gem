#!/bin/sh
# this script is intended to be run by Travis
# see https://travis-ci.org/umlaeute/Gem

## it's based on avilleret's work:
# see https://travis-ci.org/avilleret/Gem

SCRIPTDIR=${0%/*}
ENVFILE=gem.env
PDPATH=/usr/include/pd

error() {
 echo "$@" 1>&2
}
debug() {
 error "$@"
 $@
}

cd "${SCRIPTDIR}"

error "building Gem for $TRAVIS_OS_NAME"

if [ -e "${ENVFILE}" ]; then
 source "${ENVFILE}"
else
 error "couldn't read env-file: ${ENVFILE}"
fi
 

case "$TRAVIS_OS_NAME" in
 linux)
  GEMDIR=$(readlink -f "${SCRIPTDIR}/../..")
  BUILDDIR="${GEMDIR}"

  debug ${GEMDIR}/autogen.sh  || exit 1

  mkdir -p "${BUILDDIR}"
  cd "${BUILDDIR}"
  debug "${GEMDIR}/configure" --with-pd="${PDPATH}" \
	--without-ftgl \
  && make
 ;;
 osx)
  GEMDIR=$(greadlink -f "${SCRIPTDIR}/../..")
  PDDIR=/usr/include/pd
  BUILDDIR="${GEMDIR}"

  ${GEMDIR}/autogen.sh  || exit 1

  # 64bit build
  BUILDDIR="${GEMDIR}"
  PDPATH=${SCRIPTDIR}/Pd-0.46-2-64bit.app/Contents/Resources/
  mkdir -p "${BUILDDIR}"
  cd "${BUILDDIR}"
  "${GEMDIR}/configure" --with-pd="${PDPATH}" \
	--without-ftgl \
	--without-QuickTime-framework \
	--without-Carbon-framework \
  && make clean && make
  EXIT64=$?

  # 32bit build
  BUILDDIR="${GEMDIR}"
  PDPATH=${SCRIPTDIR}/Pd-0.46-2.app/Contents/Resources/
  mkdir -p "${BUILDDIR}"
  cd "${BUILDDIR}"
  "${GEMDIR}/configure" --with-pd=${PD32DIR}  --enable-fat-binary=i386 \
	--without-ftgl \
	--without-QuickTime-framework \
	--without-Carbon-framework \
  && make clean && make
  EXIT32=$?

  exit $((EXIT64+EXIT32))
 ;;
esac


