{ pkgs ? import <nixpkgs> { }, }:

let
  llvm = pkgs.llvmPackages_12;
  stdenv = llvm.libcxxStdenv;
in
stdenv.mkDerivation rec {
  pname = "WifiClient";
  version = "0.0.1";

  nativeBuildInputs = with pkgs; [
    clang-tools
    cmake
    doxygen
    gcc
    gnumake
    qtcreator
  ];

  buildInputs = with pkgs; [ qt5.full qt5.qtbase ];

  src = ./.;

  buildPhase = ''
    cmake -S$src -Bbuild
    cmake --build build
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp build/WifiClient $out/bin/WifiClient
  '';

  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath buildInputs;
}
