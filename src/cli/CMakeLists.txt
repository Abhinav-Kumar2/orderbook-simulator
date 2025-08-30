add_executable(algorithmic_trading_cli main.cpp)

target_include_directories(algorithmic_trading_cli
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# future: target_link_libraries(algorithmic_trading_cli engine strategies data)
