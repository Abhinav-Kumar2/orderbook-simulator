# Placeholder for strategies module
add_library(strategies INTERFACE)

target_include_directories(strategies
    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
)
