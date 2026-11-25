
{
  description = "Runafy - Qt6 development environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      forAllSystems = nixpkgs.lib.genAttrs systems;

    in {
      packages = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };

          qt = pkgs.qt6.env "qt6" [
            pkgs.qt6.qtbase
            pkgs.qt6.qtdeclarative
            pkgs.qt6.qttools
            pkgs.qt6.qtsvg
            pkgs.qt6.qtwayland
          ];

          pythonEnv = pkgs.python3.withPackages (ps: with ps; [
            yt-dlp
            yt-dlp-ejs
            fuzzywuzzy
          ]);

          runtime = pkgs.buildEnv {
            name = "runafy-environment";
            paths = with pkgs; [
              # C++ toolchain
              gcc
              gdb
              cmake
              ninja
              pkg-config

              # SQL
              sqlite

              # Qt
              qt
              qtcreator

              # Python
              pythonEnv

              # JavaScript challenge solving
              deno

              # Audio conversion
              ffmpeg

              # OpenGL
              libGL
              libglvnd
            ];
          };

          launcher = pkgs.writeShellScriptBin "runafy-dev" ''
            export PATH="${runtime}/bin:$PATH"

            echo "Entering Runafy development environment"
            exec bash -i
          '';

        in {
          default = pkgs.symlinkJoin {
            name = "runafy";
            paths = [ launcher runtime ];
          };
        }
      );

      apps = forAllSystems (system: {
        default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/runafy-dev";
        };
      });
    };
}
