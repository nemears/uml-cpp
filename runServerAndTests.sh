#!/bin/bash

cd build
./uml-server &
server_id=$!
./umltest
test_result=$?
kill $server_id
exit $test_result