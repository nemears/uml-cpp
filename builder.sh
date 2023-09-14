#!/bin/bash
export PATH="$cmake/bin:$gnumake/bin:$clang/bin:$coreutils/bin:$pkgconfig/bin"
export CXX="$clang/bin/clang++"
export PKG_CONFIG_PATH=$gtestdev/lib/pkgconfig
cmake $src -DNIX_BUILD=ON -DYAML_CPP_PATH=$yamlcpp -DWEBSOCKET_BUILD=ON -DCMAKE_PREFIX_PATH="$websocketpp/lib/cmake/websocketpp"
make
mkdir $out
cp uml-server libuml.a $out
