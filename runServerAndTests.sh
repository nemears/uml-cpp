#!/bin/bash
outputToNull=false
while getopts nf: flag
do
    case "${flag}" in
        n) outputToNull=true;;
        f) filter=${OPTARG};;
    esac
done
if [ -n "$filter" ] ; then
    testArgs=--gtest_filter=$filter
fi
cd build
if $outputToNull ; then
    ./uml-server > /dev/null &
else
    ./uml-server &
fi
server_id=$!
./umltest $testArgs
test_result=$?
kill $server_id
exit $test_result