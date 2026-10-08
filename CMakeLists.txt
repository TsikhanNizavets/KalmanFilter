cmake_minimum_required(VERSION 3.14)
project(KalmanFilterDemo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 3.4 REQUIRED NO_MODULE) # Add later fallback download for Eigen


# Filters
add_library(filters
    src/low_pass_filter.cpp
    src/scalar_kalman_filter.cpp
    src/linear_kalman_filter.cpp
)

target_include_directories(filters PUBLIC include)

target_link_libraries(filters PUBLIC Eigen3::Eigen)


# Innovation Analysis
add_library(innovation_analysis src/innovation_analysis.cpp)

target_include_directories(innovation_analysis PUBLIC include)

target_link_libraries(innovation_analysis PUBLIC Eigen3::Eigen)


# Tests
enable_testing()
add_subdirectory(tests)


# Examples
add_subdirectory(examples)
