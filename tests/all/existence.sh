#!/bin/sh

pd -stderr -nogui -path ../../src:../../examples/data -lib Gem existence.pd > existence.log 2>&1

cat existence.log | egrep -v "^creating: " | egrep -v "^destroying"
