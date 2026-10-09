add_library(PrecisionAPI INTERFACE)
add_library(PrecisionAPI::PrecisionAPI ALIAS PrecisionAPI)

target_include_directories(PrecisionAPI INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PrecisionAPI INTERFACE
    CommonLibSSE::CommonLibSSE
)
