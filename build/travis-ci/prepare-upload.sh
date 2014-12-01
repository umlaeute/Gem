#!/bin/sh
# this script is intended to be run by Travis
# and creates a tgz from the "make install" data.

SCRIPTDIR=$(readlink -f ${0%/*})
GEMDIR=$(readlink -f "${SCRIPTDIR}/../..")
ENVFILE=gem.env
BUILDDIR=${GEMDIR}
INSTALLDIR=

error() {
 echo "$@" 1>&2
}
debug() {
 error "$@"
 $@
}

if [ -e "${SCRIPTDIR}/${ENVFILE}" ]; then
 source "${ENVFILE}"
else
 error "couldn't read env-file: ${ENVFILE}"
fi

cd "${BUILDDIR}"

DESTDIR=$(mktemp -d /tmp/geminstall.XXXXXX)

make install DESTDIR=${DESTDIR}
cd "${DESTDIR}" && tar cvzf "${TRAVIS_BUILD_DIR}/Gem-${TRAVIS_COMMIT}.tgz"
