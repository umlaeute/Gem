#!/bin/sh
# this script is intended to be run by Travis
# and creates a tgz from the "make install" data.
test -r ${0%/*}/common.source && . ${0%/*}/common.source

cd "${BUILDDIR}"

DESTDIR=$(mktemp -d /tmp/geminstall.XXXXXX)

make install DESTDIR=${DESTDIR}/Gem pkglibdir=/ extradir=/ includedir=/develop/include pkgconfigdir=/develop/pkgconfig

cd "${DESTDIR}" && tar cvzf "${TRAVIS_BUILD_DIR}/Gem-${TRAVIS_OS_NAME}${ARCH}-${TRAVIS_TAG}.tgz" Gem/
