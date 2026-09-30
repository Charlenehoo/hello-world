cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

add_executable(hello main.cpp)
target_compile_features(hello PRIVATE cxx_std_17)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(hello PRIVATE fmt::fmt)

target_precompile_headers(hello PRIVATE pch.h)
