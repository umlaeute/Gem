#!/bin/sh
test -r ${0%/*}/common.source && . ${0%/*}/common.source

DEPPATH=${SCRIPTPATH}/deps

mkdir -p "${DEPPATH}"
cd "${DEPPATH}"

doinstall() {
  brew update
  brew install pkg-config gettext
  brew install fribidi --universal
  brew link gettext --force
  brew install imagemagick ftgl
  brew install sdl homebrew/versions/glfw2 homebrew/versions/glfw3
  brew install coreutils
  
  ## and install pd
  if [ "x${ARCH}" = "xi386" ]; then
    wget http://msp.ucsd.edu/Software/pd-0.46-2.mac.tar.gz
    tar -xf pd-0.46-2.mac.tar.gz
    PDPATH=$(pwd)/Pd-0.46-2.app/Contents/Resources/
  else
    wget http://msp.ucsd.edu/Software/pd-0.46-2-64bit.mac.tar.gz
    tar -xf pd-0.46-2-64bit.mac.tar.gz
    PDPATH=$(pwd)/Pd-0.46-2-64bit.app/Contents/Resources/
  fi
  
}

doinstall 1>&2


ENVFILE=$(mktemp /tmp/gemenv.XXXXXX)

cat > ${ENVFILE} << EOF
PDPATH=${PDPATH}
EOF

echo "${ENVFILE}"

