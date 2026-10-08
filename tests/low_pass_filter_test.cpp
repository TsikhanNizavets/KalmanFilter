#include <cmath>
#include <gtest/gtest.h>
#include "low_pass_filter.hpp"

TEST(LowPassFilterTest, ComputesTwoUpdatesAndReset) {
    constexpr float Tf = 2.0f;
    constexpr float Ts = 1.0f;
    constexpr float tolerance = 1e-6;
    constexpr float y_init = 1.0f;
    constexpr float y_reset = 10.0f;

    LowPassFilter filter(Tf, Ts, y_init); 

    const float first_estimate = filter(4.0f);
    ASSERT_TRUE(std::isfinite(first_estimate));
    EXPECT_NEAR(first_estimate, 2.0f, tolerance);

    const float second_estimate = filter(5.0f);
    ASSERT_TRUE(std::isfinite(second_estimate));
    EXPECT_NEAR(second_estimate, 3.0f, tolerance);

    filter.reset(y_reset);
    const float third_estimate = filter(4.0f);
    ASSERT_TRUE(std::isfinite(third_estimate));
    EXPECT_NEAR(third_estimate, 8.0f, tolerance);
}
