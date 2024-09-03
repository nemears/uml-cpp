{
  description = "A flake for building and developing uml-cpp with nix";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/release-24.05";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let 
          pkgs = nixpkgs.legacyPackages.${system};
          
          # function to create uml-cpp derivation from certain source
          mkUmlCpp = { src , version } : pkgs.stdenv.mkDerivation {
            inherit src;
            name = "uml-cpp";
            nativeBuildInputs = with pkgs; [pkg-config];
            buildInputs = with pkgs; [cmake pkg-config clang gnumake coreutils yaml-cpp];
            cmakeFlags = [
              "-DUML_BUILD_TESTS=OFF"
              "-DCMAKE_BUILD_TYPE=RELEASE"
            ];
            installPhase = 
            ''
              mkdir -p $out/lib/pkgconfig $out/include
              cp -r libuml.so $out/lib
              cp -r $src/include/uml $out/include
              touch $out/lib/pkgconfig/uml-cpp.pc
              echo "prefix=$out

              Name: uml
              Description: uml modeling C++ api
              Version: ${version}
              Libs: -L$out/lib -luml
              Cflags: -I$out/include" > $out/lib/pkgconfig/uml-cpp.pc

              export PKG_CONFIG_PATH=PKG_CONFIG_PATH:$out/lib/pkgconfig
            '';
          };
        in
        {
          # dev shell for nix develop (good for commandline dev)
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [ 
              # build dependencies
              clang
              cmake 
              pkg-config
              gtest
              yaml-cpp

              # dev dependencies
              websocketpp 
              asio
              openssl

              # debug dependencies
              gdb
              valgrind
            ];
            stdenv = pkgs.clangStdenv;
          };

          packages = {
            uml-cpp = mkUmlCpp {
              src = ./.;
              version = "0.4.1";
            };
            uml-cpp_0_3_5 = mkUmlCpp {
              src = pkgs.fetchFromGitHub {
                owner = "nemears";
                repo = "uml-cpp";
                rev = "v0.3.5";
                hash = "sha256-HzzOe+9s67LwWlHWYIa+vxbWMBVCsVY7RXfFDkElwpY=";
              };
              version = "0.3.5";
            };
            default = self.packages.${system}.uml-cpp;
          };
          overlay = final : prev: {
            uml-cpp = self.packages.${system}.uml-cpp;
            uml-cpp_0_3_5 = self.packages.${system}.uml-cpp_0_3_5;
          };
        }
      );
}
