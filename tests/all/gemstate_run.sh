#!/bin/sh

#echo "(not) running test: $@"
#exit 0

if test "x${PD}" = "x"
then
 PD=pd
fi
#PDARGS="-noprefs -stderr"
#PDARGS="${PDARGS} -oss -nosound -nrt"
#PDARGS="${PDARGS} -nogui"
#PDARGS="${PDARGS} -path ../../abstractions:../../:../../examples/data -lib Gem"


obj=$1
patch=$2
order=$3
echo "testing ${patch} ($order) with ${obj}..." 1>&2
${PD} -open "object_gemstate.${patch}.pd" -send "order $(echo $order | grep -o .)" -send "object ${obj}" 2>&1
RET=$?
if [ ${RET} != 0 ]; then
echo "ERR: tested ${patch} ($order) with ${obj}...: ${RET}"
fi
