#!/bin/bash

echo "running clang format. pwd: $(pwd)"
mkdir -p patches/src/pde
mkdir -p patches/src/device
for file in $(find src -type f)
do
  echo ${file}
  patch="./patches/${file}-format.patch"
  diff ${file} <(clang-format ${file}) >> ${patch}
  patch ${file} ${patch}
done
