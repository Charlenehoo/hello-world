cmake_minimum_required(VERSION 3.16)
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Charlene Hoo")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in"
    "${VERSION_HEADER}"
    @ONLY
)

add_library("${PROJECT_NAME}" SHARED src/main.cpp)
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE
        /utf-8
        /Zc:__cplusplus
        /permissive-
    )
endif()

target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/src"
)

set(SKYRIM_SUPPORT_AE ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE" CommonLibSSE EXCLUDE_FROM_ALL SYSTEM)
find_package(commonlib-shared REQUIRED CONFIG)
target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
    CommonLibSSE::CommonLibSSE
)

target_precompile_headers("${PROJECT_NAME}" PRIVATE src/pch.h)
