#!/bin/sh
# this script is intended to be run by Travis
# see https://travis-ci.org/umlaeute/Gem

## it's based on avilleret's work:
# see https://travis-ci.org/avilleret/Gem

test -r ${0%/*}/common.source && . ${0%/*}/common.source

cd "${SCRIPTDIR}"

error "building Gem for $TRAVIS_OS_NAME"

debug ${GEMDIR}/autogen.sh  || exit 1
mkdir -p "${BUILDDIR}"
cd "${BUILDDIR}"

case "$TRAVIS_OS_NAME" in
    linux)
        if [ "x${ARCH}" != "x" -a "x${ARCH}" != "x$(uname -m)" ]; then
          error "unable to cross-compile for architecture ${ARCH}"
          exit 1
        fi
	CONFIGUREFLAGS="--without-ftgl"
	;;
    osx)
	CONFIGUREFLAGS="--without-ftgl \
           --without-QuickTime-framework \
           --without-Carbon-framework"
	if [ "x${ARCH}" != "x" ]; then
	    CONFIGUREFLAGS+=" --enable-fat-binary=${ARCH}"
	fi
	;;
esac

debug "${GEMDIR}/configure" --with-pd="${PDPATH}" ${CONFIGUREFLAGS} \
      && make

