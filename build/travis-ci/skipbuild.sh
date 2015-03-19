#!/bin/sh

error() {
  echo "$@" 1>&2
}

## skip coverity_scan builds on anything but linux
if [ "x${TRAVIS_OS_NAME}" = "xosx" -a "x${COVERITY_SCAN_BRANCH}" = "x1" ]; then
  error "skipping coverity_scan build on OSX"
  exit 0
fi

exit 1
