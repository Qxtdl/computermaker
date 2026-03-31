{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  name = "devshell";
  packages = with pkgs; [
    # one time use
    # bear

    glfw
    cglm
    gnumake
    # gcc
  ];
}
