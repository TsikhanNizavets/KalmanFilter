# Visual filter test
add_executable(2dtest
    2dtest.cpp
)

target_link_libraries(2dtest PRIVATE kalman_filters)

target_compile_definitions(2dtest PRIVATE TEST2D_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/2dtest_data.csv")

# Mehra test
add_executable(mehra_test
    mehra_test.cpp
)

target_link_libraries(mehra_test PRIVATE kalman_filters innovation_analysis)

target_compile_definitions(mehra_test PRIVATE MEHRA_TEST_PATH="${CMAKE_CURRENT_BINARY_DIR}/mehra_test_data.csv")
