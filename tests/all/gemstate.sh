#!/bin/sh

if test "x${PD}" = "x"
then
 PD=pd
fi

LOGFILE="gemstate.log"

PDARGS="-noprefs -stderr"
PDARGS="${PDARGS} -oss -nosound -nrt"
PDARGS="${PDARGS} -nogui"
PDARGS="${PDARGS} -path ../../abstractions:../../:../../examples/data -lib Gem"
OBJECTS="objects.txt"

if [ -r "$1" ]; then
 OBJECTS=$1
fi

TESTPATCHES="          \
	nop      \
	_object_pix  \
	_object_tex  \
	"

ORDERS="OWRo WORo WROo OWRrwo OWRrow WORrwo WORrow WROrwo WROrow"

rm -f "${LOGFILE}"
cat "${OBJECTS}" | sed -e 's|;$||' | while read obj
do
 for patch in ${TESTPATCHES}; do
  for order in ${ORDERS}; do
    echo "testing ${patch} ($order) with ${obj}..." 1>&2
    if test ! -e "${patch}.pd" ; then
     patch="t a"
    fi
    sed -e "s|@PRERENDER@|${patch}|g" object_gemstate.template > object_gemstate.pd
    order=$(echo $order | grep -o .)
    "${PD}" ${PDARGS} -open object_gemstate.pd -send "order ${order}" -send "object ${obj}" 2>&1
  done
 done
done >> "${LOGFILE}"
