#!/bin/sh

SCRIPTPATH=${0%/*}
ASTYLECONF=${SCRIPTPATH}/../../doc/astyle.rc

do_indent() {
 astyle --options=${ASTYLECONF} $1
}

for i in $@
do
  if [ -f "${i}" ]; then
    do_indent "${i}"
  else
    echo "file does not exist: ${i}" 1>&2
  fi
done
