cmake_minimum_required(VERSION 3.16)
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Charlene Hoo")
set(PROJECT_AUTHOR_EMAIL "CharleneHoo@hotmail.com")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in"
    "${VERSION_HEADER}"
    @ONLY
)

add_library(
    "${PROJECT_NAME}" SHARED
    src/main.cpp
    src/Event/EventProcessor.cpp
)
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE
        /utf-8
        /Zc:__cplusplus
        /permissive-
    )
endif()

target_include_directories(
    "${PROJECT_NAME}" PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/src"
)

set(SKYRIM_SUPPORT_AE ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE" CommonLibSSE EXCLUDE_FROM_ALL SYSTEM)
if(TARGET CommonLibSSE)
    target_compile_options(CommonLibSSE PRIVATE
        $<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>:-w>
        $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-w>
        $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/w>
    )
endif()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(
    "${PROJECT_NAME}" PRIVATE
    CommonLibSSE::CommonLibSSE
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
)

target_precompile_headers("${PROJECT_NAME}" PRIVATE src/pch.h)

if(DEFINED ENV{MO2})
    set(MO2_PLUGIN_DIR "$ENV{MO2}/mods/${PROJECT_NAME}/SKSE/Plugins")

    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MO2_PLUGIN_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${MO2_PLUGIN_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMENT "Copying $<TARGET_FILE_NAME:${PROJECT_NAME}> to MO2: ${MO2_PLUGIN_DIR}"
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
            "${MO2_PLUGIN_DIR}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            COMMENT "Copying $<TARGET_PDB_FILE_NAME:${PROJECT_NAME}> to MO2: ${MO2_PLUGIN_DIR}"
        )
    endif()

    message(STATUS "MO2 detected. Output will be copied to: ${MO2_PLUGIN_DIR}")
else()
    message(STATUS "MO2 environment variable not found. Skipping auto-copy.")
endif()
