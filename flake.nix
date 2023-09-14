{
  description = "A flake for building and developing uml-cpp with nix";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/release-23.05";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in
        {
          # dev shell for nix develop (good for commandline dev)
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [ 
              # build dependencies
              cmake 
              clang
              gnumake
              pkg-config

              # dev dependencies
              websocketpp 
              asio
              openssl
            ];
          };
          
          # build flake with nix build
          packages.default = derivation {
            name = "uml-cpp";
            src = ./.;
            inherit system;
            inherit (pkgs) cmake pkg-config clang gnumake coreutils gtest yaml-cpp websocketpp;
            # environment variables for builder
            yamlcpp = pkgs.yaml-cpp;
            gtestdev = pkgs.gtest.dev;
            pkgconfig = pkgs.pkg-config;
            # build with bash and builder script
            builder = "${pkgs.bash}/bin/bash";
            args = [ ./builder.sh ];
          };
        }
      );
}
