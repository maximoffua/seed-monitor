{
  pkgs,
  lib,
  config,
  ...
}: let
  pio = import ./modules/platformio.nix {inherit pkgs;};
  gcc-arm = pio.mkPlatformIOPackage "toolchain-gccarmnoneeabi" [pkgs.gcc-arm-embedded];
  openocd = pio.mkPlatformIOPackage "tool-openocd" [pkgs.openocd];
in {
  imports = [
    ./modules/toolchain.nix
  ];

  env.VENV_PATH = "${config.env.DEVENV_STATE}/venv";

  languages.python = {
    enable = true;
    venv.enable = true;
    package = pkgs.python3.withPackages (ps:
      with ps; [
        pyserial
        pkgs.platformio
      ]);
  };
  languages.go.enable = true;

  languages.platformio = {
    enable = true;
    platformPackages = [gcc-arm openocd];
  };
  languages.nix.enable = true;

  packages = [
    pkgs.gcc-arm-embedded
    pkgs.openocd
    pkgs.cobra-cli
    pkgs.influxdb2
  ];

  enterShell = ''
    pio init
    export PYTHONPATH="$VENV_PATH/lib/python3.11/site-packages:$PYTHONPATH"
  '';

  scripts.build.exec = lib.mkDefault "pio run -e $1";
  scripts.upload.exec = lib.mkDefault "pio run -e $1 -t upload";

  processes.influxdb2.exec = "influxd";

  # https://devenv.sh/pre-commit-hooks/
  # pre-commit.hooks.shellcheck.enable = true;

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";

  # See full reference at https://devenv.sh/reference/options/
}
