#!/bin/sh

## puts dependencies besides the binary

## usage: $0 <binary> [<binary2>...]

error() {
   echo "$@" 1>&2
}

NTLDD=$(which ntldd 2>/dev/null)

if [ "x${NTLDD}" = "x" ]; then
  error "no 'ntldd' binary found"
  exit 0
fi


list_deps() {
  ${NTLDD} -R "$1" \
	| grep -i mingw \
	| awk '{print $3}' \
	| grep -i mingw \
	| sed -e 's|\\|/|g'
}

install_deps () {
outdir=$2
if [ "x${outdir}" = "x" ]; then
  outdir=${1%/*}
fi
if [ "x${outdir}" = "x" ]; then
  outdir=.
fi

list_deps "$1" | while read dep; do
  depfile=$(basename "${dep}")
  if [ -e "${outdir}/${depfile}" ]; then
    error "skipping already localized depdendency ${dep}"
  else
    cp -v "${dep}" "${outdir}"
  fi
done

}


for f in "$@"; do
   if [ -e "${f}" ]; then
       install_deps "${f}"
   fi
done
