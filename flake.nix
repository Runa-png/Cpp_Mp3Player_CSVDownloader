

{
  description = "Qt6 C++ MP3 Player and CSV Downloader";

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
          pkgs = import nixpkgs {
            inherit system;
          };

          pythonEnv = pkgs.python3.withPackages (ps: with ps; [
            yt-dlp
            yt-dlp-ejs
            fuzzywuzzy
          ]);

          app = pkgs.stdenv.mkDerivation {
            pname = "cpp-mp3player-csvdownloader";
            version = "1.0.0";

            src = self;

            nativeBuildInputs = with pkgs; [
              cmake
              ninja
              pkg-config
              qt6.wrapQtAppsHook
              makeWrapper
            ];

            buildInputs = with pkgs; [
              # Qt6
              qt6.qtbase
              qt6.qtdeclarative
              qt6.qttools
              qt6.qtsvg
              qt6.qtwayland

              # Python
              pythonEnv

              # SQL
              sqlite

              # Media and JavaScript
              ffmpeg
              deno

              # Graphics
              libGL
              libglvnd
            ];

            cmakeBuildType = "Release";

            configurePhase = ''
              runHook preConfigure

              cmake -S . -B build -G Ninja \
                -DCMAKE_BUILD_TYPE=Release

              runHook postConfigure
            '';

            buildPhase = ''
              runHook preBuild
              cmake --build build
              runHook postBuild
            '';

            installPhase = ''
              runHook preInstall

              mkdir -p $out/bin
              install -Dm755 build/app $out/bin/app

              runHook postInstall
            '';

            postFixup = ''
              wrapProgram $out/bin/app \
                --prefix PATH : ${pkgs.lib.makeBinPath [
                  pythonEnv
                  pkgs.ffmpeg
                  pkgs.deno
                ]}
            '';
          };

        in {
          default = app;
        }
      );

      apps = forAllSystems (system: {
        default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/app";
        };
      });

      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };

          pythonEnv = pkgs.python3.withPackages (ps: with ps; [
            yt-dlp
            yt-dlp-ejs
            fuzzywuzzy
          ]);

        in {
          default = pkgs.mkShell {
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
              qt6.qtbase
              qt6.qtdeclarative
              qt6.qttools
              qt6.qtsvg
              qt6.qtwayland
              qtcreator

              # Python
              pythonEnv

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
        }
      );
    };
}
