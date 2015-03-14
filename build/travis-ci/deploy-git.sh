#!/bin/sh
# this script is intended to be run by Travis
# and creates a tgz from the "make install" data.
test -r ${0%/*}/common.source && . ${0%/*}/common.source

if [ "x${GITDEPLOYTARGET}" = "x" ]; then
 error "no git-deploytarget defined; skipping deployment"
 exit 0
fi
GITDEPLOYHOST="${GITDEPLOYTARGET##*@}"
GITDEPLOYHOST="${GITDEPLOYHOST%%/*}"

KEYFILE=~/.ssh/id_rsa

OS_NAME="${TRAVIS_OS_NAME}"
if [ "x${OS_NAME}" = "x" ]; then OS_NAME=OS; fi
if [ "x$(which dpkg-architecture)" != "x" ]; then
 if [ "x${ARCH}" = "x" ]; then ARCH=$(dpkg-architecture -qDEB_BUILD_ARCH); fi
fi
if [ "x$(which uname)" != "x" ]; then
 if [ "x${ARCH}" = "x" ]; then ARCH=$(uname -m); fi
fi
if [ "x${ARCH}" = "x" ]; then ARCH=default; fi
if [ "x${BUILDDIR}" = "x" ]; then BUILDDIR=.; fi

DEPLOY_BRANCH="${TRAVIS_OS_NAME}/${ARCH}"


error "deploying to branch: ${DEPLOY_BRANCH}"


## config done

## password-less authentication to deploy host

# make sure our remote host is known
# (so we are not asked to accept it)
mkdir -p ~/.ssh
chmod 700 ~/.ssh
ssh-keyscan -H ${GITDEPLOYHOST} >> ~/.ssh/known_hosts
error "ssh-keyscanned ${GITDEPLOYHOST}"

# and use the (encrypted) auth key
if [ -e "${KEYFILE}" ]; then
 chmod 600 "${KEYFILE}"
 ssh-add "${KEYFILE}"
 error "ssh-added ${KEYFILE}"
else
 error "missing ${KEYFILE}"
fi

## prepare the target directory
# (fetch it via git)
cd "${BUILDDIR}"
BUILDDIR=$(pwd)

DESTDIR=$(mktemp -d /tmp/geminstall.XXXXXX)

git clone ${GITDEPLOYTARGET} "${DESTDIR}"
cd "${DESTDIR}"
git checkout "${DEPLOY_BRANCH}" || git checkout -b "${DEPLOY_BRANCH}"
rm -rf *
cd "${BUILDDIR}"


error "now installing Gem to ${DESTDIR}"
make install DESTDIR=${DESTDIR}/Gem pkglibdir=/ extradir=/ includedir=/develop/include pkgconfigdir=/develop/pkgconfig

cd "${DESTDIR}"
error "setting up user-identity"
git config user.email "zmoelnig@travis-ci.umlaeute.mur.at"
git config user.name "Travis CI"


error "commiting Gem-${DEPLOY_BRANCH}"
git add -A .
git commit -m "Gem ${TRAVIS_TAG} ${DEPLOY_BRANCH}

built '${TRAVIS_TAG}' for ${DEPLOY_BRANCH}"
error "now pushing Gem from $(pwd)"
git push --set-upstream origin "${DEPLOY_BRANCH}"
error "$0 done."
