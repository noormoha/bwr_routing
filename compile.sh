#!/bin/bash
set -v #echo on

sudo rm -r -f CMakeFiles
sudo rm -f CMakeCache.txt
sudo rm -f MakeFile
sudo rm -f cmake_install.cmake

CC=gcc CXX=g++ cmake -DCMAKE_BUILD_TYPE=Release .

make

#rm -r -f CMakeFiles
#rm -f CMakeCache.txt
