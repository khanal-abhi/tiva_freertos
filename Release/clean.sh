#!/bin/sh
cmake .. -DCMAKE_BUILD_TYPE=Release
make clean
rm -rf MakeFile
