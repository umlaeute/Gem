#!/bin/sh

patch=$1
plugtype=$2
patch=${patch%/MAIN.pd}/MAIN.pd
test=$(echo "${patch%MAIN.pd}" | sed -e 's|/*$||' -e 's|.*/||')
echo "TEST: ${test}"
if [ -z "${test}" ]; then
    test="test$$"
fi
echo "TEST: ${test}"
testdir="$(pwd)/${test}"

usage() {
    cat >/dev/stderr <<EOF
USAGE
       $0 <testpatch> [<plugtype>]

       this runs the given <testpatch> once for each plugin (of the given type)

       this script must be run from a directory containing the 'devgem.sh' startup script
       to launch Pd with the current Gem

       it also depends on all to-be-tested plugins to be found in the 'backends/' directory.
       you can pass an optional <plugtype> to filter for a subset of available plugins

HOW IT WORKS
       first move all plugins ('gem_*.so' files) from your current directory
       into a 'backends/' directory.
       then run the script.
       this will copy the (matching) plugins one by one from the 'backends/' to the pwd
       and run the test.
       after that, the copied plugin is removed.
       if you need additional plugins to be present, you can leave them in the currect
       directory - just make sure they don't match the tested plugins (otherwise they)
       will be removed.

       for each test an output directory is created (using the last directory component
       of the patch as a name, so 'tests/manual/pix-bla/MAIN.pd' will be found in 'pix-bla/')
       the full path along with the plugin name under test is sent to the test-patch via the
       'autotest' receiver. (e.g. testing the gem_imageTIFF.so plugin with the pix-bla test
       will send '$(pwd)/pix-bla TIFF').
       the test is expected to put any relevant information into that directory,
       so you can later inspect the output.
       

EXAMPLE
       $0 tests/manual/pix-writer/MAIN.pd image

       this runs the 'pix-writer' test with all 'gem_image*' plugins found in backends/

SIDE EFFECTS
       running this script removes plugins in the current directory

EOF

if [ $# -gt 0 ]; then
    cat >/dev/stderr <<EOF
ERROR
       $@

please fix the error above and re-run the script.
EOF
    fi
exit 1

}

test -x devgem.sh || usage "'devgem.sh' not found in the current directory"
test -d backends || usage "'backends/' not found in the current directory"
test -e "${patch}" || usage "you must provide a patch to run"
for plugin in "backends/gem_${plugtype}"*.so; do :; done
test -e "${plugin}" || usage "no backend of type '${plugtype}' found"


mkdir -p "${testdir}"
for plugin in "backends/gem_${plugtype}"*.*; do
  plugfile=${plugin##*/}
  p=${plugfile#gem_image}
  p=${p%.*}
  rm -f "${plugfile}"
  cp "${plugin}" .
  ./devgem.sh -nogui -open "${patch}" -send "autotest ${testdir} ${p}"
  rm -f "${plugfile}"
done
