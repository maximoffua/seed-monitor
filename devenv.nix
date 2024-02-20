{pkgs, lib, config, ...}:
let
    pio = import ./modules/platformio.nix { inherit pkgs; };
    gcc-arm = pio.mkPlatformIOPackage "toolchain-gccarmnoneeabi" [pkgs.gcc-arm-embedded];
    openocd = pio.mkPlatformIOPackage "tool-openocd" [pkgs.openocd];
in
 {
 imports = [
    ./modules/toolchain.nix
  ];

  languages.python = {
    enable = true;
    package = pkgs.python3.withPackages (ps: with ps; [
        pyserial
        pkgs.platformio
    ]);
  };

  languages.platformio = {
    enable = true;
    platformPackages = [ gcc-arm openocd ];
  };
  languages.nix.enable = true;

  packages = [
  pkgs.gcc-arm-embedded
  pkgs.openocd
  ];

  enterShell = ''
    pio init
  '';

  scripts.build.exec = "pio run -e $1";

  # https://devenv.sh/pre-commit-hooks/
  # pre-commit.hooks.shellcheck.enable = true;

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";

  # See full reference at https://devenv.sh/reference/options/
}
