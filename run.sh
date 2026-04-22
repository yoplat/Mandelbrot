#!/usr/bin/zsh
mkdir -p build && cd src
clang++ -O3 -Wall -Wextra -o ../build/main main.cpp mandelbrot.cpp
cd ..
./build/main
