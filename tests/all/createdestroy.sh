#!/bin/sh

if test "x${PD}" = "x"
then
 PD=pd
fi

LOGFILE="createdestroy.log"

PDARGS="-noprefs -stderr -nogui"
PDARGS="${PDARGS} -nogui"
PDARGS="${PDARGS} -path ../../abstractions:../../:../../examples/data -lib Gem"

TESTPATCHES="                 \
	createdestroy.pd      \
	createdestroy_AB.pd   \
	createdestroy_AxB.pd  \
	createdestroy_AxBy.pd \
	createdestroy_AxyB.pd \
	createdestroy_xAB.pd  \
	createdestroy_xABy.pd \
	createdestroy_xAyB.pd \
	"

rm -f "${LOGFILE}"
cat objects.txt | sed -e 's|;$||' | while read obj
do
 for patch in ${TESTPATCHES}; do
  if test -e "${patch}" ; then
    echo "testing ${patch} with ${obj}..." 1>&2
    "${PD}" ${PDARGS} -open ${patch} -send "object ${obj}" 2>&1
  fi
 done
done >> "${LOGFILE}"
