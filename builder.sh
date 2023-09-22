#!/bin/bash
export PATH="$cmake/bin:$gnumake/bin:$clang/bin:$coreutils/bin:$pkgconfig/bin"
export CXX="$clang/bin/clang++"
export PKG_CONFIG_PATH=$gtestdev/lib/pkgconfig:$asio/lib/pkgconfig:$openssldev/lib/pkgconfig
cmake $src -DNIX_BUILD=ON -DYAML_CPP_PATH=$yamlcpp -DWEBSOCKET_BUILD=ON -DWEBSOCKETPP_DIR="$websocketpp/lib/cmake/websocketpp" -DCMAKE_PREFIX_PATH="$websocketpp/lib/cmake/websocketpp:$cmake"
make
mkdir -p $out/bin $out/lib
cp -r libuml.a libuml-kitchen-cpp.a $src/include $out/lib
cp uml-server $out/bin
