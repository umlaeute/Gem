#!/bin/sh
# this script is intended to be run by Travis
# and creates a tgz from the "make install" data.
test -r ${0%/*}/common.source && . ${0%/*}/common.source

if [ "x${GITDEPLOYTARGET}" = "x" ]; then
 error "no git-deploytarget defined; skipping deployment"
 exit 0
fi

KEYFILE=${0%/*}/travisci.pem
chmod 600 "${KEYFILE}"
ssh-add "${KEYFILE}"

OS_NAME="${TRAVIS_OS_NAME}"
if [ "x${OS_NAME}" = "x" ]; then OS_NAME=OS; fi
if [ "x${ARCH}" = "x" ]; then ARCH=default; fi
if [ "x${BUILDDIR}" = "x" ]; then BUILDDIR=.; fi

DEPLOY_TAG="${TRAVIS_OS_NAME}/${ARCH}"

cd "${BUILDDIR}"
BUILDDIR=$(pwd)

DESTDIR=$(mktemp -d /tmp/geminstall.XXXXXX)

git clone ${GITDEPLOYTARGET} "${DESTDIR}"
cd "${DESTDIR}"
git checkout "${DEPLOY_TAG}" || git checkout -b "${DEPLOY_TAG}"
rm -rf *
cd "${BUILDDIR}"



make install DESTDIR=${DESTDIR}/Gem pkglibdir=/ extradir=/ includedir=/develop/include pkgconfigdir=/develop/pkgconfig

cd "${DESTDIR}"
git add -A .
git commit -m "built '${TRAVIS_TAG}' for ${DEPLOY_TAG}"
git push --set-upstream origin "${DEPLOY_TAG}"
