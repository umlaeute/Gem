#!/bin/sh
# this script is intended to be run by Travis
# see https://travis-ci.org/umlaeute/Gem

## it's based on avilleret's work:
# see https://travis-ci.org/avilleret/Gem

test -r ${0%/*}/common.source && . ${0%/*}/common.source

cd "${SCRIPTDIR}"

error "building Gem for $TRAVIS_OS_NAME"

case "$TRAVIS_OS_NAME" in
 linux)
  debug ${GEMDIR}/autogen.sh  || exit 1

  mkdir -p "${BUILDDIR}"
  cd "${BUILDDIR}"
  debug "${GEMDIR}/configure" --with-pd="${PDPATH}" \
	--without-ftgl \
  && make
 ;;
 osx)
  debug ${GEMDIR}/autogen.sh  || exit 1

  # 64bit build
  mkdir -p "${BUILDDIR}"
  cd "${BUILDDIR}"
  debug "${GEMDIR}/configure" --with-pd="${PDPATH}" \
	--without-ftgl \
	--without-QuickTime-framework \
	--without-Carbon-framework \
  && make clean && make
  EXIT64=$?

  # 32bit build
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


