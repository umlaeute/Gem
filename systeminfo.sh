#!/bin/sh

get_cpus() {
 # default value
 CPU_COUNT=0

 if [ $((CPU_COUNT)) -lt 1 ]; then
  if [ -r /proc/cpuinfo ]; then
   CPU_COUNT=$(grep -c "^processor	*:" /proc/cpuinfo)
  fi
 fi
 if [ $((CPU_COUNT)) -lt 1 ]; then
  if [ "x$(which sysctl)" != "x" ]; then
   CPU_COUNT=$(sysctl -n hw.ncpu)
  fi
 fi



 if [ $((CPU_COUNT)) -lt 1 ]; then
   # there's at least 1 CPU...
   CPU_COUNT=1
 fi
 echo $((CPU_COUNT + 0))
}


echo "CPUs: $(get_cpus)"
