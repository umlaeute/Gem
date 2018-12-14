#!/bin/sh
test -r ${0%/*}/common.source && . ${0%/*}/common.source

DEPDIR=${SCRIPTDIR}/deps

mkdir -p "${DEPDIR}"
cd "${DEPDIR}"

## LATER: fetch the lastest GemDependencies package via
github_list_releaseartifacts() {
 # call as: github_list_releaseartifacts umlaeute/Gem-dependencies
 curl -s https://api.github.com/repos/$1/releases/latest \
	| jq -r ".assets[] | select(.name | test(\"${spruce_type}\")) | .browser_download_url"
}

brewinstall() {
  brew install "$@" || (brew upgrade "$@" && brew cleanup "$@")
}

doinstall() {
  brew update
  brewinstall pkg-config gettext
  brew link gettext --force
  if [ "x${ARCH}" != "xi386" ]; then
    brewinstall fribidi
  fi
  brewinstall imagemagick ftgl
  brewinstall sdl glfw glfw@2
  brewinstall coreutils
  
  ## and install pd
  PDVERSION="0.48-0"
  if [ "x${ARCH}" = "xi386" ]; then
    wget http://msp.ucsd.edu/Software/pd-${PDVERSION}-i386.mac.tar.gz
    tar -xf pd-${PDVERSION}-i386.mac.tar.gz
    PDDIR=$(pwd)/Pd-${PDVERSION}-i386.app/Contents/Resources/
  else
    wget http://msp.ucsd.edu/Software/pd-${PDVERSION}.mac.tar.gz
    tar -xf pd-${PDVERSION}.mac.tar.gz
    PDDIR=$(pwd)/Pd-${PDVERSION}.app/Contents/Resources/
  fi
  chmod -R u+rXw .
  
}

doinstall 1>&2


ENVFILE=$(mktemp /tmp/gemenv.XXXXXX)

cat > ${ENVFILE} << EOF
PDDIR=${PDDIR}
EOF

echo "${ENVFILE}"

