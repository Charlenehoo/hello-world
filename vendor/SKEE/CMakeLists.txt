add_library(SKEE INTERFACE)
add_library(SKEE::SKEE ALIAS SKEE)

target_include_directories(SKEE SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(SKEE INTERFACE
    CommonLibSSE::CommonLibSSE
)
