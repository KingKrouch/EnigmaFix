{ pkgs ? import <nixpkgs> {} , buildType ? "Debug" }:

let
  # Import the cross compilation environment for mingw-w64 targeting 64-bit Windows
  pkgsCross = import <nixpkgs> { crossSystem = { config = "x86_64-w64-mingw32"; }; };
  stdenvCross = pkgsCross.stdenv;  # Reference stdenv from pkgsCross
in
stdenvCross.mkDerivation rec {
  name = "build-${buildType}";

  src = ./.;  # Use the current directory as the source

  buildInputs = [
    pkgs.git
    pkgs.cmake
    pkgs.ninja
    pkgs.meson
    pkgs.pkgsCross.mingwW64.gcc
    pkgs.boost
  ];

  buildPhase = ''
    cmake -B Intermediate/${buildType} -G Ninja -DCMAKE_BUILD_TYPE=${buildType}
  '';

  installPhase = ''
    cd Intermediate/${buildType} && ninja
  '';

  meta = with pkgsCross.lib; {  # Use pkgsCross.lib here
    description = "A mod that aims to improve several aspects of the PC versions for games in the Death end re;Quest series";
    license = licenses.mit;
    maintainers = [ "KingKrouch" ];
    platforms = platforms.windows;
  };
}