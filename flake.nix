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
          buildUmlCpp = { src , version } : pkgs.stdenv.mkDerivation {
            inherit src;
            name = "uml-cpp";
            nativeBuildInputs = with pkgs; [pkg-config];
            buildInputs = with pkgs; [cmake pkg-config clang gnumake coreutils yaml-cpp];
            cmakeFlags = [
              "-DUML_BUILD_TESTS=OFF"
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
              cmake 
              clang
              gnumake
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
          };

          packages.uml-cpp = buildUmlCpp {
            src = ./.;
            version = "0.4.0";
          };
          packages.uml-cpp_0_3_6 = buildUmlCpp {
            src = pkgs.fetchFromGitHub {
              owner = "nemears";
              repo = "uml-cpp";
              rev = "v0.3.6";
              hash = "sha256-crZCrVuUpW217dQ4WuN8qAcVtbMZSsbKhhHnnC8OEDc=";
            };
            version = "0.3.6";
          };
          packages.default = self.packages.${system}.uml-cpp;
          overlay = final : prev: {
            uml-cpp = self.packages.${system}.uml-cpp;
            uml-cpp_0_3_6 = self.packages.${system}.uml-cpp_0_3_6;
          };
        }
      );
}
