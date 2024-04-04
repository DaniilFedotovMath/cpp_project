cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CLANG_DEFAULT_CXX_STDLIB libc++)

project(statball LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(.)
include_directories(third_party)

add_executable(fedotov
    impl/main.cpp
    impl/correlation.cpp
    impl/heun.cpp
    impl/laplacian.cpp
    impl/modelling.cpp
    impl/wave_function.cpp)
