#!/bin/sh
cmake .. -DCMAKE_BUILD_TYPE=Debug
make clean
rm -rf MakeFile
