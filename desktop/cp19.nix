# Nix is a powerful package manager for Linux and other Unix systems that makes
# package management reliable and reproducible: https://nixos.org/nix/.
# This file is intended to be used with `nix-shell`
# (https://nixos.org/nix/manual/#sec-nix-shell) to setup a fully-functional
# LEDE build environment by installing all required dependencies.
with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "cp19-dev-env";
  buildInputs = [
    gnumake
    gcc
		gnumake pkg-config wayland wayland-scanner wayland-utils glew-egl
		libxkbcommon
		boost
		cmake
    python3Minimal
  ];
}
