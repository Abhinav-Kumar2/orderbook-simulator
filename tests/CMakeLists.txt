enable_testing()

add_executable(dummy_test dummy_test.cpp)
target_link_libraries(dummy_test PRIVATE Catch2::Catch2WithMain data)

include(CTest)
include(Catch)

catch_discover_tests(dummy_test)
