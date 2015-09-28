#!/usr/bin/env bash

# This wrapper should solve the issue where a Windows script (.bat) cannot be successfully executed if the current directory is a UNC directory

winpath() { printf "%s" "${1//\//\\}"; }

cd / && cmd <<EOT
  pushd $(winpath "$OLDPWD")
  $@
EOT
