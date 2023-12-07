#!/bin/sh

cd build
make install
cd ..
cp uml-cpp.pc /usr/lib/pkgconfig/uml-cpp.pc