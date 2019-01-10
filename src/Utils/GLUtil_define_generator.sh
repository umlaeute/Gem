#!/bin/bash
grep define ../Gem/glew.h |
grep -v _MESA |
grep -v GLEXT_PROTOTYPES |
grep "GL_" |
awk '{print $2}' |
egrep "^GL_" |
awk '{print length($1),$1}' |
sort -n |
sed -e 's|\([0-9]*\) \(.*\)$|\1 \2\n\1|' |
sed -e :a -e '/^\([0-9]*\)$/N; s/\n/ /; ta' |
awk '{if (3==NF){if ($1!=$2) print $2,$3; else print $3}else if (2==NF)print $1,$2}' |
awk '{if (2==NF){print "        return _GL_UNDEFINED;\n    case "$1":"; $1=$2}; print "#ifdef "$1"\n       if (!strcmp(name, \""$1"\")) return "$1";\n#endif"}'
