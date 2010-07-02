#!/bin/sh

if test "x${GEM_VERSION_MAJOR}" = "x"; then
 GEM_VERSION_MAJOR="0"
fi
if test "x${GEM_VERSION_MINOR}" = "x"; then
 GEM_VERSION_MINOR="93"
fi


subversion_version () {
    GEM_VERSION_CODENAME="SVN"
    GEM_VERSION_BUGFIX="0"
    if which svnversion 2>&1 > /dev/null; then
      GEM_VERSION_BUGFIX="rev$(svnversion .)"
    fi
}


fullversion() {
                if test "x${GEM_VERSION_BUGFIX}" = "x"; then
		  echo "${GEM_VERSION_MAJOR}.${GEM_VERSION_MINOR} ${GEM_VERSION_CODENAME}"
		else
		  echo "${GEM_VERSION_MAJOR}.${GEM_VERSION_MINOR}.${GEM_VERSION_BUGFIX} ${GEM_VERSION_CODENAME}"
		fi
}

substitute_file() {
 local INFILE
 local OUTFILE
 OUTFILE=$1
 INFILE=${OUTFILE}.in

 if [ -e "${INFILE}" ]; then
   sed  -e "s|@GEM_VERSION_MAJOR@|${GEM_VERSION_MAJOR}|g" \
	-e "s|@GEM_VERSION_MINOR@|${GEM_VERSION_MINOR}|g" \
	-e "s|@GEM_VERSION_BUGFIX@|${GEM_VERSION_BUGFIX}|g" \
	-e "s|@GEM_VERSION_CODENAME@|${GEM_VERSION_CODENAME}|g" \
	${INFILE} > ${OUTFILE}
 else
  echo "${INFILE} not found!"

 fi

}

if test "x${GEM_VERSION_BUGFIX}" = "x"; then
  if [ -d .svn ]; then
    subversion_version
  fi
fi

if test "x$1" = "x"; then
  fullversion
else
 case $1 in
	--major)
		echo ${GEM_VERSION_MAJOR}
	;;
	--minor)
		echo ${GEM_VERSION_MINOR}
	;;
	--bugfix)
		echo ${GEM_VERSION_BUGFIX}
	;;
	--codename)
		echo ${GEM_VERSION_CODENAME}
	;;
	*)
		substitute_file $1	
	;;
 esac
fi
