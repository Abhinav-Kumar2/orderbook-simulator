# Placeholder for engine module
add_library(engine INTERFACE)

target_include_directories(engine
    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
)
