enable_testing()
add_executable(dummy_test dummy_test.cpp)
add_test(NAME DummyTest COMMAND dummy_test)
