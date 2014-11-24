#!/bin/sh

if test "x${PD}" = "x"
then
 PD=pd
fi
PDARGS="-noprefs -stderr -nogui"
PDARGS="${PDARGS} -nogui"
PDARGS="${PDARGS} -path ../../abstractions:../../:../../examples/data -lib Gem"

"${PD}" ${PDARGS} -open existence.pd -send "pd quit"> existence.log 2>&1

cat existence.log | egrep -v "^creating: " | egrep -v "^destroying"
