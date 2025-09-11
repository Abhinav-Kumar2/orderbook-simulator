cmake_minimum_required(VERSION 3.15)
project(algorithmic_trading_engine_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)

include(FetchContent)
FetchContent_Declare(
  sqlite3
  GIT_REPOSITORY https://github.com/sqlite/sqlite.git
  GIT_TAG version-3.46.0
)
FetchContent_MakeAvailable(sqlite3)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(src/cli)
add_subdirectory(src/data)
add_subdirectory(src/engine)
add_subdirectory(src/strategies)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
