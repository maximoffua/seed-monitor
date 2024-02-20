{pkgs, config, lib, ...}:
let
    cfg = config.languages.platformio;
    utils = import ./platformio.nix { inherit pkgs; };
    inherit (utils) platformioConfig;
in
{
  options.languages.platformio = {
    enable = lib.mkEnableOption "PlatformIO project integration and tooling";

    package = lib.mkOption {
      type = lib.types.package;
      default = pkgs.platformio;
      defaultText = lib.literalExpression "pkgs.platformio";
      description = "The PlatformIO package to use.";
    };

    platformPackages = lib.mkOption {
      type = lib.types.listOf lib.types.package;
      default = [];
      description = "Native packages to use with PlatformIO.";
    };

    overrides = lib.mkOption {
      type = lib.types.package;
      default = null;
      internal = true;
    };
  };

  config = lib.mkIf cfg.enable {
    packages = [
      cfg.package
    ] ++ cfg.platformPackages;

    languages.platformio.overrides = platformioConfig cfg.platformPackages;

    enterShell = let ini = platformioConfig cfg.platformPackages; in ''
    mkdir -p $DEVENV_STATE/platformio/packages
    ${if (!builtins.isNull ini) then "install -v ${ini} $DEVENV_STATE/platformio/overrides.ini" else ""}
    '';
  };
}