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
	object_gemstate_simple.pd      \
	object_pix.pd  \
	"

ORDERS="OWRrwo OWRrow WORrwo WORrow WROrwo WROrow"

rm -f "${LOGFILE}"
cat "${OBJECTS}" | sed -e 's|;$||' | while read obj
do
 for patch in ${TESTPATCHES}; do
  for order in ${ORDERS}; do
   if test -e "${patch}" ; then
    echo "testing ${patch} ($order) with ${obj}..." 1>&2
    order=$(echo $order | grep -o .)
    "${PD}" ${PDARGS} -open ${patch} -send "order ${order}" -send "object ${obj}" 2>&1
   fi
  done
 done
done >> "${LOGFILE}"
