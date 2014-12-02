#!/bin/sh
# this script is intended to be run by Travis
# and creates a tgz from the "make install" data.
test -r ${0%/*}/common.source && . ${0%/*}/common.source

cd "${BUILDDIR}"

DESTDIR=$(mktemp -d /tmp/geminstall.XXXXXX)

make install DESTDIR=${DESTDIR}

cd "/tmp/" && tar cvzf "${TRAVIS_BUILD_DIR}/Gem-${TRAVIS_COMMIT}.tgz" "${DESTDIR#/tmp/}"
