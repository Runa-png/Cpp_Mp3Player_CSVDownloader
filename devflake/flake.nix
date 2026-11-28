
{
  description = "Qt6 C++ development environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };

      qt = pkgs.qt6.env "qt6" [
        pkgs.qt6.qtbase
        pkgs.qt6.qtdeclarative
        pkgs.qt6.qttools
        pkgs.qt6.qtsvg
        pkgs.qt6.qtwayland
      ];

      yt-dlp-latest = pkgs.python3Packages.buildPythonApplication {
        pname = "yt-dlp";
        version = "2026.11.23";
        pyproject = true;

        src = pkgs.fetchPypi {
          pname = "yt_dlp";
          version = "2026.11.23";
          hash = "sha256-Hbg3cqjeeFWi2rVRqU1/YXyIYp399QZTHAazabR/iKQ=";
        };

        build-system = with pkgs.python3Packages; [
          hatchling
        ];

        dependencies = [
          pkgs.python3Packages.yt-dlp-ejs
        ];

        pythonImportsCheck = [ "yt_dlp" ];
        doCheck = false;
      };

      pythonEnv = pkgs.python3.withPackages (ps: [
        ps.fuzzywuzzy
      ]);

    in {
      packages.${system}.yt-dlp-latest = yt-dlp-latest;

      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
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

          # yt-dlp
          yt-dlp-latest

          # YouTube JavaScript challenge solving
          deno

          # Audio conversion
          ffmpeg

          # OpenGL
          libGL
          libglvnd
        ];

        shellHook = ''
          echo "Entering Qt6 development environment"
        '';
      };
    };
}
