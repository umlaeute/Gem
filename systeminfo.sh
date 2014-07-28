#!/bin/sh


## get full OS name
get_os__linux() {
   if [ "x$(which lsb_release)" != "x" ]; then
      lsb_release -sd
   fi
}
get_os__OSX() {
   if [ "x$(which sw_vers)" != "x" ]; then
      echo "$(sw_vers -productName) $(sw_vers -productVersion) ($(sw_vers -buildVersion))"
   fi
}
get_os() {
 # default value
 GUESS_OS=""

 if [ "x${GUESS_OS}" = "x" ]; then
   GUESS_OS=$(get_os__linux)
 fi
 if [ "x${GUESS_OS}" = "x" ]; then
   GUESS_OS=$(get_os__OSX)
 fi

 if [ "x${GUESS_OS}" = "x" ]; then
   # weird OS...
   GUESS_OS="<unknown>"
 fi
 echo ${GUESS_OS}
}


## get kernel version
get_kernel__uname() {
   if [ "x$(which uname)" != "x" ]; then
      uname -a
   fi
}
get_kernel() {
 # default value
 GUESS_KERNEL=""

 if [ "x${GUESS_KERNEL}" = "x" ]; then
   GUESS_KERNEL=$(get_kernel__uname)
 fi

 if [ "x${GUESS_KERNEL}" = "x" ]; then
   # weird KERNEL...
   GUESS_KERNEL="<unknown>"
 fi
 echo ${GUESS_KERNEL}
}

## get number of CPUs available
get_cpus__proc() {
  if [ -r /proc/cpuinfo ]; then
   echo $(grep -c "^processor	*:" /proc/cpuinfo)
  fi
}
get_cpus__OSX() {
  if [ "x$(which sysctl)" != "x" ]; then
   sysctl -n hw.ncpu
  fi
}

get_cpus() {
 # default value
 CPU_COUNT=0

 if [ $((CPU_COUNT)) -lt 1 ]; then
   CPU_COUNT=$(get_cpus__proc)
 fi
 if [ $((CPU_COUNT)) -lt 1 ]; then
   CPU_COUNT=$(get_cpus__proc)
 fi

 if [ $((CPU_COUNT)) -lt 1 ]; then
   # there's at least 1 CPU...
   CPU_COUNT=1
 fi
 echo $((CPU_COUNT + 0))
}


echo "HOST-OS    : $(get_os)"
echo "HOST-KERNEL: $(get_kernel)"
echo "HOST-#CPU  : $(get_cpus)"
