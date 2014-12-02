#!/bin/sh
test -r ${0%/*}/common.source && . ${0%/*}/common.source

cd "${SCRIPTDIR}"
INSTALLSCRIPT="./depinstall-${TRAVIS_OS_NAME}.sh"

if [ -x "${INSTALLSCRIPT}" ]; then
 envfile=$(${INSTALLSCRIPT})
 mv "${envfile}" "${ENVFILE}"
else
 error "no install-script found for: ${TRAVIS_OS_NAME}"
 error "assuming all is well"
fi


