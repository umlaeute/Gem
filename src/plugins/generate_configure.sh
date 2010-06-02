#!/bin/sh

DEFAULTOUTFILE=configure.ac

listplugins () {
 for f in */configure.ac
 do
  d=${f%/configure.ac}
  if egrep -w "^${d}" .svn/entries  > /dev/null 2>&1; then echo "${d}"; fi
 done
}

generate_body () {
 listplugins | sort -u | while read d
 do
 name=$(echo ${d} | sed -e 's|\([a-z]*\)\(.*\)|\1: \2|')
cat <<ACEOM
### ${name}
if test -d "\$srcdir/${d}"; then
  AC_CONFIG_SUBDIRS([${d}])
fi

ACEOM
 done
}


generate_header () {
cat <<ACEOM
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(Gem-plugins,[],[zmoelnig@iem.at])

AC_CONFIG_SRCDIR([Makefile])
ACEOM
}
generate_footer () {
cat <<ACEOM

AC_OUTPUT

ACEOM
}


generate_configure () {
 generate_header
 generate_body
 generate_footer
}

if test -t 1
then
 generate_configure > ${DEFAULTOUTFILE}
 echo "generated new autoconf file as '${DEFAULTOUTFILE}'" 1>&2
else
 generate_configure 
fi
