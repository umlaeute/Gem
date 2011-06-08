#!/bin/sh

TEMPLATEFILE=Makefile.am.template

if [ -e "${TEMPLATEFILE}" ]; then
 :
else
 LIBRARYNAME=$(pwd | sed -e 's|^.*/||')

echo '
AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = lib@LTLIBRARYNAME@.la

lib@LTLIBRARYNAME@_la_CXXFLAGS =
lib@LTLIBRARYNAME@_la_LIBADD   =

# RTE flags
lib@LTLIBRARYNAME@_la_CXXFLAGS += @GEM_RTE_CFLAGS@
lib@LTLIBRARYNAME@_la_LIBADD   += @GEM_RTE_LIBS@

lib@LTLIBRARYNAME@_la_SOURCES= @SOURCES@
'| sed "s|@LTLIBRARYNAME@|${LIBRARYNAME}|g" > ${TEMPLATEFILE}

fi

echo "# this file was generated automatically from ${TEMPLATEFILE}
# rather than editing this file directly, you should instead edit the
# original ${TEMPLATEFILE} and re-generate this file" | \
cat - ${TEMPLATEFILE} | \
sed -e s/@SOURCES@/"$(ls *.cpp *.h | sort | sed -e 's/^/ \\\\\\n    /g' | tr -d '\n')"/
