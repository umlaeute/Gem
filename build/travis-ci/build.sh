#!/bin/sh
# this script is intended to be run by Travis
# see https://travis-ci.org/umlaeute/Gem

## it's based on avilleret's work:
# see https://travis-ci.org/avilleret/Gem

SCRIPTDIR=${0%/*}
ENVFILE=gem.env

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
  BUILDDIR="${SCRIPTDIR}/${TRAVIS_OS_NAME}-amd64"

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
  BUILDDIR="${TRAVIS_OS_NAME}-amd64"

  ${GEMDIR}/autogen.sh  || exit 1

  # 64bit build
  BUILDDIR="${TRAVIS_OS_NAME}-amd64"
  PDDIR=${SCRIPTDIR}/Pd-0.46-2-64bit.app/Contents/Resources/
  mkdir -p "${SCRIPTDIR}/${BUILDDIR}"
  cd "${SCRIPTDIR}/${BUILDDIR}"
  "${GEMDIR}/configure" --with-pd="${PDPATH}" \
	--without-ftgl \
	--without-QuickTime-framework \
	--without-Carbon-framework \
  && make
  EXIT64=$?

  # 32bit build
  BUILDDIR="${TRAVIS_OS_NAME}-i386"
  PDDIR=${SCRIPTDIR}/Pd-0.46-2.app/Contents/Resources/
  mkdir -p "${SCRIPTDIR}/${BUILDDIR}"
  cd "${SCRIPTDIR}/${BUILDDIR}"
  "${GEMDIR}/configure" --with-pd=${PD32DIR}  --enable-fat-binary=i386 \
	--without-ftgl \
	--without-QuickTime-framework \
	--without-Carbon-framework \
  && make
  EXIT32=$?

  exit $((EXIT64+EXIT32))
 ;;
esac


