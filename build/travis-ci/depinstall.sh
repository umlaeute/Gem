#!/bin/sh
SCRIPTDIR=${0%/*}
cd "${SCRIPTDIR}"
INSTALLSCRIPT="./depinstall-${TRAVIS_OS_NAME}.sh"

if [ -x "${INSTALLSCRIPT}" ]; then
 envfile=$(${INSTALLSCRIPT})
 mv "${envfile}" gem.env
else
 echo "no install-script found for: ${TRAVIS_OS_NAME}" 1>&2
 echo "assuming all is well" 1>&2
fi


