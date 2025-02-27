{ pkgs ? import <nixpkgs> {} , buildType ? "Debug" }:

let
  # Import the cross compilation environment for mingw-w64 targeting 64-bit Windows
  pkgsCross = import <nixpkgs> {
    crossSystem = { config = "x86_64-w64-mingw32"; };
  };
  stdenvCross = pkgsCross.stdenv;  # Reference stdenv from pkgsCross
in
stdenvCross.mkDerivation rec {
  name = "build-${buildType}";

  src = ./.;  # Use the current directory as the source

  buildInputs = [
    pkgs.pkgsCross.mingwW64.buildPackages.gcc
    pkgs.boost
    pkgs.zydis
  ];

  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    git
    meson
  ];

  configurePhase = "
    rm -rf Intermediate
    cmake -B Intermediate/${buildType} -G Ninja -DCMAKE_BUILD_TYPE=${buildType}
  ";

  buildPhase = ''
    cd Intermediate/${buildType} && ninja
  '';

  installPhase = ''
    mkdir -p $out
    cp -r /build/EnigmaFix/Binaries/Windows $out/
  '';

  meta = with pkgsCross.lib; {  # Use pkgsCross.lib here
    description = "A mod that aims to improve several aspects of the PC versions for games in the Death end re;Quest series";
    license = licenses.mit;
    maintainers = [ "KingKrouch" ];
    platforms = platforms.windows;
  };
}
