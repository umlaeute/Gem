#!/bin/sh

if test "x${GEM_MAJOR_VERSION}" = "x"; then
 GEM_MAJOR_VERSION="0"
fi
if test "x${GEM_MINOR_VERSION}" = "x"; then
 GEM_MINOR_VERSION="93"
fi


subversion_version () {
    GEM_CODENAME="SVN"
    GEM_BUGFIX_VERSION="0"
    if which svnversion 2>&1 > /dev/null; then
      GEM_BUGFIX_VERSION=$(svnversion .)
    fi
}



if test "x${GEM_BUGFIX_VERSION}" = "x"; then
  if [ -d .svn ]; then
    subversion_version
  fi
fi

case $1 in
	--major)
		echo ${GEM_MAJOR_VERSION}
	;;
	--minor)
		echo ${GEM_MINOR_VERSION}
	;;
	--bugfix)
		echo ${GEM_BUGFIX_VERSION}
	;;
	--codename)
		echo ${GEM_CODENAME}
	;;
	*)
		echo "${GEM_MAJOR_VERSION}.${GEM_MINOR_VERSION}.${GEM_BUGFIX_VERSION} ${GEM_CODENAME}"
	;;
esac
