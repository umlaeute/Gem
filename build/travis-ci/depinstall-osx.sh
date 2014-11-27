#!/bin/sh

DEPPATH=${0%/*}/deps
mkdir -p "${DEPPATH}"
cd "${DEPPATH}"

doinstall() {
  brew update
  brew install pkg-config gettext
  brew install fribidi --universal
  brew link gettext --force
  brew install imagemagick ftgl
  brew install sdl homebrew/versions/glfw2 homebrew/versions/glfw3
  brew install sshpass
  
  ## and install pd
  wget http://msp.ucsd.edu/Software/pd-0.46-2.mac.tar.gz
  wget http://msp.ucsd.edu/Software/pd-0.46-2-64bit.mac.tar.gz
  
  tar -xf pd-0.46-2.mac.tar.gz
  tar -xf pd-0.46-2-64bit.mac.tar.gz
}

doinstall 1>&2


ENVFILE=$(mktemp /tmp/gemenv.XXXXXX)

cat > ${ENVFILE} << EOF
PD32PATH=$(pwd)/Pd-0.46-2.app/Contents/Resources/
PDPATH=$(pwd)/Pd-0.46-2-64bit.app/Contents/Resources/
EOF

echo "${ENVFILE}"

