# Tracking 2D
add_executable(tracking_2d
    tracking_2d.cpp
)

target_link_libraries(tracking_2d PRIVATE filters)

target_compile_definitions(tracking_2d PRIVATE TRACKING_2D_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/tracking_2d_data.csv")


# Filter Comparison
add_executable(filter_comparison
    filter_comparison.cpp
    data_generation/synthetic_data.cpp
)
target_link_libraries(filter_comparison PRIVATE filters)

target_compile_definitions(filter_comparison PRIVATE FILTER_COMPARISON_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/filter_comparison_data.csv")



# Mehra Whiteness
add_executable(mehra_whiteness
    mehra_whiteness_demo.cpp
    data_generation/synthetic_data.cpp
)
target_link_libraries(mehra_whiteness PRIVATE filters innovation_analysis)

target_compile_definitions(mehra_whiteness PRIVATE MEHRA_WHITENESS_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/mehra_whiteness_data.csv"
                                                NORMALIZED_AUTOCORRELATION_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/normalized_autocorrelation_data.csv")
