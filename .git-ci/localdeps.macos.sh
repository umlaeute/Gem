#!/bin/bash

## puts dependencies besides the binary
# LATER: put dependencies into a separate folder

## usage: $0 <binary> [<binary2>...]

error() {
   echo "$@" 1>&2
}
basename () {
  local x=${1##*/}
  if [ "x$x" = "x" ]; then
     echo $1
  else
     echo $x
  fi
}
dirname () {
  local x=${1%/*}
  if [ "x$x" = "x" ]; then
     echo .
  else
     echo $x
  fi
}

if [ "x${otool}" = "x" ]; then
 otool="otool -L"
fi
if [ "x${otool}" = "x" ]; then
  error "no 'otool' binary found"
  exit 0
fi

list_deps() {
  $otool "$1" \
        | grep -v ":$" \
	| grep compatibility \
	| awk '{print $1}' \
	| egrep '^/' \
 	| egrep -v '^/usr/lib/' \
	| egrep -v '^/System/Library/Frameworks'
}

install_deps () {
error "DEP: ${INSTALLDEPS_INDENT}$1"
outdir=$2
if [ "x${outdir}" = "x" ]; then
  outdir=$(dirname "$1")
fi
if [ ! -d "${outdir}" ]; then
  outdir=.
fi

list_deps "$1" | while read dep; do
  infile=$(basename "$1")
  depfile=$(basename "${dep}")
  install_name_tool -change "${dep}" "@loader_path/${depfile}" "$1"

  if [ -e "${outdir}/${depfile}" ]; then
    error "DEP: ${INSTALLDEPS_INDENT}  ${dep} SKIPPED"
  else
    error "DEP: ${INSTALLDEPS_INDENT}  ${dep} -> ${outdir}"
    cp "${dep}" "${outdir}"
    chmod u+w "${outdir}/${depfile}"
    install_name_tool -id "@loader_path/${depfile}" "${outdir}/${depfile}"
    # recursively call ourselves, to resolve higher-order dependencies
    INSTALLDEPS_INDENT="${INSTALLDEPS_INDENT}    " $0 "${outdir}/${depfile}"
  fi
done

}


for f in "$@"; do
   if [ -e "${f}" ]; then
       install_deps "${f}"
   fi
done
