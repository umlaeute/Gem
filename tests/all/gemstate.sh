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
PD="${PD} ${PDARGS}"


OBJECT_FILE="objects.txt"

if [ -r "$1" ]; then
 OBJECT_FILE=$1
fi

if [ "x${TESTPATCHES}" = "x" ]; then
TESTPATCHES="          \
	nop      \
	_object_pix  \
	_object_tex  \
	"
fi

if [ "x${ORDERS}" = "x" ]; then
 ORDERS="${ORDERS} Oo OWo OWow OWwo WOo WOow WOwo"
 ORDERS="${ORDERS} OWRo WORo WROo OWRrwo OWRrow WORrwo WORrow WROrwo WROrow"
fi



for patch in ${TESTPATCHES}; do
    prerender=${patch}
    if test ! -e "${patch}.pd" ; then
     prerender="t a"
    fi
    sed -e "s|@PRERENDER@|${prerender}|g" object_gemstate.template > "object_gemstate.${patch}.pd"
done


rm -f "${LOGFILE}"
if which parallel >/dev/null; then
 echo "parallel found"
 PD=${PD} parallel ./gemstate_run.sh :::: "${OBJECT_FILE}" ::: ${TESTPATCHES} ::: ${ORDERS} >> "${LOGFILE}"
else
 cat "${OBJECT_FILE}" | while read obj
 do
  for patch in ${TESTPATCHES}; do
   for order in ${ORDERS}; do
     echo "testing ${patch} ($order) with ${obj}..." 1>&2
     order=$(echo $order | grep -o .)
     #"${PD}" ${PDARGS} -open "object_gemstate.${patch}.pd" -send "order ${order}" -send "object ${obj}" 2>&1
     PD=${PD} ./gemstate_run.sh "${obj}" "${patch}" "${order}" >> "${LOGFILE}"
   done
  done
 done >> "${LOGFILE}"
fi
