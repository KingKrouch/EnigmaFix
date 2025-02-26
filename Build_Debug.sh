#!/usr/bin/env bash
# Check the distribution ID in /etc/*release
DISTRO_ID=$(cat /etc/*release | grep ^DISTRIB_ID= | cut -d '=' -f 2 | tr -d '"')

# Check if the system is NixOS by looking for "nixos"
if [ "$DISTRO_ID" == "nixos" ]; then
  echo "This system is NixOS. Building through nix-build..."
  NIXPKGS_ALLOW_UNSUPPORTED_SYSTEM=1 nix-build --arg buildType '"Debug"'
else
  echo "This system is not NixOS. Building natively..."
  cmake -B Intermediate/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug && cd Intermediate/Debug && ninja
fi