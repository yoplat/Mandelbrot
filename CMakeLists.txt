cmake_minimum_required(VERSION 3.18)
project(mandelbrot LANGUAGES C CXX) # C needed for glad

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---- Shared deps ----
find_package(Threads REQUIRED)

# ---- Shared compile options (interface target) ----
# projects flags is a 'false' lib used to add common flags to a config
add_library(project_flags INTERFACE)
target_compile_options(project_flags INTERFACE
    -Wall -Wextra
    $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native>
    $<$<CONFIG:Debug>:-O0 -g -fsanitize=address,undefined>
)
target_link_options(project_flags INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# ---- CPU executable ----
add_executable(mandelbrot_cpu
    src/main_cpu.cpp
    src/mandelbrot.cpp
    src/third_party/stb_image_write.cpp
)
target_include_directories(mandelbrot_cpu PRIVATE src)
target_link_libraries(mandelbrot_cpu PRIVATE
    Threads::Threads
    project_flags
)

# ---- GPU executable ----
find_package(PkgConfig REQUIRED) # used to check packages
pkg_check_modules(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)

add_executable(mandelbrot_gpu
    src/main_gpu.cpp
    src/third_party/glad.c
)
target_include_directories(mandelbrot_gpu PRIVATE
    src
    include
    ${GLFW_INCLUDE_DIRS}
)
target_link_libraries(mandelbrot_gpu PRIVATE
    ${GLFW_LIBRARIES}
    OpenGL::GL
    m
    dl
    project_flags
)
