#!/bin/bash

cd build
./uml-server &
server_id=$!
./umltest
kill $server_id