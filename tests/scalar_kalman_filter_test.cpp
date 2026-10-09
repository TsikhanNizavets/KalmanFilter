#include "scalar_kalman_filter.hpp"

#include <cmath>

#include <gtest/gtest.h>

TEST(ScalarKalmanFilterTest, ComputesTwoUpdates) {
    constexpr float initial_estimate = 0.0f;
    constexpr float initial_uncertainty = 1.0f;
    constexpr float measurement_variance = 1.0f;
    constexpr float process_variance = 1.0f;
    constexpr float dt = 1.0f;
    constexpr float tolerance = 0.00001f;

    ScalarKalmanFilter filter(initial_estimate, initial_uncertainty,
                         measurement_variance, process_variance, dt);

    const float first_estimate = filter(4.0f);
    ASSERT_TRUE(std::isfinite(first_estimate));
    EXPECT_NEAR(first_estimate, 8.0f / 3.0f, tolerance);

    const float second_estimate = filter(4.0f);
    ASSERT_TRUE(std::isfinite(second_estimate));
    EXPECT_NEAR(second_estimate, 3.5f, tolerance);
}
