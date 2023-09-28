#!/bin/bash

set -e

. assert.sh

echo "Tests sur les damiers"

for testfile in `ls TEST10/*.test`
do
   imagefile=${testfile%.test}.png
   assert "./raytrace.sh $testfile" ""
   assert "./compare.sh $imagefile ${imagefile#TEST10/}" "OK\n0\n"
done

assert_end regression
