add_executable(2dtest
    2dtest.cpp
)

target_link_libraries(2dtest PRIVATE kalman_filters)

target_compile_definitions(2dtest PRIVATE TEST2D_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/2dtest_data.csv")
