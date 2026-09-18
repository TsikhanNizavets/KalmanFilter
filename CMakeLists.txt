cmake_minimum_required(VERSION 3.14)
project(KalmanFilterDemo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile source files into the filter_demo executable
add_executable(filter_demo
    src/main.cpp
    src/LowPassFilter.cpp
    src/kalman_filter_1d.cpp
)

target_include_directories(filter_demo PRIVATE include)

target_compile_definitions(filter_demo PRIVATE SENSOR_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/sensor_data.csv")
