#!/bin/sh

PUSH=
if [ "x$1" = "x-n" ]; then
 PUSH=no
fi
if [ "x$1" = "x-y" ]; then
 PUSH=yes
fi

if git push . master:coverity_scan
then
 if [ "x${PUSH}" = "x" ]; then
  echo -n "Shall I push 'coverity_scan' to remote [Y/n]? " 1>&2
  read PUSH
  if [ "x${PUSH}" = "x" ]; then
    PUSH=yes
  fi
  case "${PUSH}" in
    yes|YES|y|Y)
      PUSH=yes
      ;;
    no|NO|n|N)
      PUSH=no
      ;;
    *)
      echo "Invalid value '${PUSH}' - assuming 'no'" 1>&2
      PUSH=no
      ;;
  esac
 fi
 if [ "x${PUSH}" = "xyes" ]; then
  git push origin coverity_scan:coverity_scan
 fi
fi
