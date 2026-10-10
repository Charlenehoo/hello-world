add_library(NGDAPI INTERFACE)
add_library(NGD::NGD ALIAS NGDAPI)

target_include_directories(NGDAPI SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(NGDAPI INTERFACE
    CommonLibSSE::CommonLibSSE
)
