{
  pkgs,
  ...
}: let
  toolchainTemplate = name:
    builtins.toJSON {
      inherit name;
      version = "0.0.0";
      description = "Fake toolchain to use system compiler";
      homepage = "https://nixos.org";
      license = "MIT";
      system = ["linux_x86_64"];
      repository = {
        type = "git";
        url = "https://github.com";
      };
      keywords = ["toolchain" "build tools" "compiler" "assembler" "linker" "preprocessor"];
    };
  mkPlatformIOPackage = name: deps:
    pkgs.stdenv.mkDerivation {
      pname = name;
      version = "0.0.0";
      # src = pkgs.writeText "package.json" (toolchainTemplate name);
      buildInputs = deps;
      unpackPhase = "true";
      installPhase = ''
        mkdir -p $out/${name}
        # install -Dm644 package.json $out/${name}/package.json
        cat <<EOF > $out/${name}/package.json
        ${toolchainTemplate name}
        EOF
      '';
      passthru = {
        inherit name;
      };
      meta = {
        description = "Fake toolchain to use system binaries";
        homepage = "https://nixos.org";
        license = "MIT";
        system = ["linux_x86_64"];
        repository = {
          type = "git";
          url = "https://github.com";
        };
        keywords = ["toolchain" "build tools" "compiler" "assembler" "linker" "preprocessor"];
      };
    };
  platformioConfig = packages:
    if (builtins.length packages > 0)
    then
      pkgs.writeText "overrides.ini" ''
        [platformio]
        packages_dir = ''${sysenv.DEVENV_STATE}/platformio/packages
        [env]
        platform_packages =
            ${builtins.concatStringsSep "\n    " (builtins.map (p: "${p.name}@symlink://${p}/${p.name}") packages)}
      ''
    else null;
in {
  inherit mkPlatformIOPackage platformioConfig toolchainTemplate;
}
