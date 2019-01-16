#!/bin/sh

test -r ${0%/*}/common.source && . ${0%/*}/common.source

DEPDIR=${SCRIPTDIR}/deps
mkdir -p "${DEPDIR}"
cd "${DEPDIR}"

doinstall() {
  sudo ../depinstall-linux.sudo.sh

  #wget http://msp.ucsd.edu/Software/pd-0.46-2.src.tar.gz
  #tar -xvf pd-0.46-2.src.tar.gz
  #PDDIR=$(pwd)/pd-0.46-2/src
}

doinstall 1>&2

ENVFILE=$(mktemp /tmp/gemenv.XXXXXX)

cat > ${ENVFILE} << EOF
PDDIR=${PDDIR}
EOF

echo "${ENVFILE}"


