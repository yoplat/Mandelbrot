#!/usr/bin/zsh
mkdir -p build && cd src
clang++ -Wall -Wextra -o ../build/main main.cpp mandelbrot.cpp
cd ..
./build/main
