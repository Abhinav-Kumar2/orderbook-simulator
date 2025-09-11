add_library(data STATIC DataStore.cpp)
target_include_directories(data PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(data PUBLIC sqlite3)
