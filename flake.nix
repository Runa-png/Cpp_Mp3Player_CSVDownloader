
{
  description = "Qt6 C++ MP3 Player";

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

          app = pkgs.stdenv.mkDerivation {
            pname = "cpp-mp3player-csvdownloader";
            version = "1.0.0";

            src = self;

            nativeBuildInputs = with pkgs; [
              cmake
              ninja
              pkg-config
              qt.wrapQtAppsHook
            ];

            buildInputs = with pkgs; [
              qt
              pythonEnv
              sqlite
              ffmpeg
              deno
              libGL
              libglvnd
            ];

            cmakeBuildType = "Release";

            installPhase = ''
              runHook preInstall

              mkdir -p $out/bin
              cp build/app $out/bin/app

              runHook postInstall
            '';

            dontWrapQtApps = false;
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
    };
}

