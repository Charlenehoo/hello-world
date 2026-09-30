cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

add_executable(hello main.cpp)
target_compile_features(hello PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(hello PRIVATE /utf-8)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE" CommonLibSSE EXCLUDE_FROM_ALL)
find_package(commonlib-shared REQUIRED CONFIG)
target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
    CommonLibSSE::CommonLibSSE
)

target_precompile_headers("${PROJECT_NAME}" PRIVATE pch.h)
