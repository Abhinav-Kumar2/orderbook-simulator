cmake_minimum_required(VERSION 3.15)
project(algorithmic_trading_engine_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)

add_subdirectory(src/cli)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
