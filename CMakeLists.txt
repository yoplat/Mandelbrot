cmake_minimum_required(VERSION 3.18)
project(mandelbrot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for clangd in Neovim

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

add_executable(mandelbrot
    src/main.cpp
    src/mandelbrot.cpp
    src/third_party/stb_image_write.cpp
)

target_include_directories(mandelbrot PRIVATE src)
target_link_libraries(mandelbrot PRIVATE Threads::Threads)

# Warnings always on
target_compile_options(mandelbrot PRIVATE -Wall -Wextra)

# Release flags
target_compile_options(mandelbrot PRIVATE
    $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native>
    $<$<CONFIG:Debug>:-O0 -g -fsanitize=address,undefined>
)
target_link_options(mandelbrot PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
