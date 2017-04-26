#!/bin/sh
test -r ${0%/*}/common.source && . ${0%/*}/common.source

DEPDIR=${SCRIPTDIR}/deps

mkdir -p "${DEPDIR}"
cd "${DEPDIR}"

brew-install() {
  brew install "$@" || (brew upgrade "$@" && brew cleanup "$@")
}

doinstall() {
  brew update
  brew-install pkg-config gettext
  brew-install fribidi --universal
  brew-link gettext --force
  brew-install imagemagick ftgl
  brew-install sdl glfw glfw@2
  brew-install coreutils
  
  ## and install pd
  PDVERSION="0.46-5"
  if [ "x${ARCH}" = "xi386" ]; then
    wget http://msp.ucsd.edu/Software/pd-${PDVERSION}.mac.tar.gz
    tar -xf pd-${PDVERSION}.mac.tar.gz
    PDDIR=$(pwd)/Pd-${PDVERSION}.app/Contents/Resources/
  else
    wget http://msp.ucsd.edu/Software/pd-${PDVERSION}-64bit.mac.tar.gz
    tar -xf pd-${PDVERSION}-64bit.mac.tar.gz
    PDDIR=$(pwd)/Pd-${PDVERSION}-64bit.app/Contents/Resources/
  fi
  chmod -R u+rXw .
  
}

doinstall 1>&2


ENVFILE=$(mktemp /tmp/gemenv.XXXXXX)

cat > ${ENVFILE} << EOF
PDDIR=${PDDIR}
EOF

echo "${ENVFILE}"

