#!/bin/sh

: "${CC:=cc}"

usage() {
     arch="$(${CC} -dumpmachine)"
cat <<EOF
usage: $0 [-a <arch>] <file> [<file> ...]

renames each <file> to include .<arch> just before the file-extension.
the no <arch> is specified, use $arch.

E.g.
  foo.so -> foo.${arch}.so
  bar    -> bar.${arch}

EOF

if [ -n "$1" ]; then
    exit "$((1+0))"
fi
exit
}

addinfix() {
    local f
    local d
    local b
    local x
    f="$1"
    d="${f%/*}"
    if [ "${d}" = "${f}" ]; then
	d="."
    else
	f="${f##*/}"
    fi
    b="${f%.*}"
    if [ "${b}" = "${f}" ]; then
	x=""
    else
	x=".${f##*.}"
    fi
    mv "$1" "${d}/${b}.${2}${x}"
}

while getopts "ha:" arg; do
    case $arg in
	h)
	    usage
	    ;;
	a)
	    arch="${OPTARG}"
	    ;;
	*)
	    usage 1
	    ;;
    esac
done
shift $((OPTIND-1))
test -n "$*" || usage 1

: "${arch:=$(${CC} -dumpmachine)}"
if [ -z "${arch}" ]; then
    echo "$0: Unable to determine <arch>" 1>&2
    exit 1
fi

for f in "$@"; do
    addinfix "$f" "${arch}"
done
