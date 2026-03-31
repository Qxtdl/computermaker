{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  name = "devshell";
  packages = with pkgs; [
    glfw
    cglm
    gnumake

    # one time use
    # bear
  ];
}
