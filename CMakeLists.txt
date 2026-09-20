cmake_minimum_required(VERSION 3.14)
project(KalmanFilterDemo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 3.4 REQUIRED NO_MODULE) # Add later fallback download for Eigen

# Core: kalman_filters
add_library(kalman_filters
    src/low_pass_filter.cpp
    src/kalman_filter_1d.cpp
    src/kalman_filter_constant_acceleration_1d.cpp
)
target_include_directories(kalman_filters PUBLIC include)
target_link_libraries(kalman_filters PUBLIC Eigen3::Eigen)

# Main
add_executable(filter_demo src/main.cpp)
target_link_libraries(filter_demo PRIVATE kalman_filters)

target_compile_definitions(filter_demo PRIVATE SENSOR_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/sensor_data.csv")

# Test
add_subdirectory(test)
