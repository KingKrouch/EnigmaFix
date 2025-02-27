#!/usr/bin/env bash
# Check the distribution ID in /etc/*release
DISTRO_NAME=$(cat /etc/*release | grep ^NAME= | cut -d '=' -f 2 | tr -d '"')

# Check if the system is NixOS by looking for "NixOS"
if [ "$DISTRO_NAME" == "NixOS" ]; then
  echo "This system is NixOS. Building through nix-build..."
  NIXPKGS_ALLOW_UNSUPPORTED_SYSTEM=1 nix-build --arg buildType '"Debug"'
else
  echo "This system is not NixOS. Building natively..."
  rm -rf Intermediate
  cmake -B Intermediate/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug && cd Intermediate/Debug && ninja
fi