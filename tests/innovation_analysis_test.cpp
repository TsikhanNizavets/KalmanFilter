#include <cmath>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>
#include "innovation_analysis.hpp"

TEST(InnovationAnalysisTest, FailSequence) {
    constexpr int total_samples = 100;
    constexpr int max_lag = 1;
    constexpr int num_innovations_to_skip = 0; 
    constexpr float tolerance = 0.00001f;
    std::vector<Eigen::VectorXf> innovations;

    for (int i = 0; i < total_samples; ++i) {
        innovations.push_back(Eigen::Vector<float, 1> {1 - 2 * (i % 2)});
    }

    auto result = InnovationAnalysis::analyze(innovations, max_lag, num_innovations_to_skip);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[0](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[0](0, 0), 1.0f, tolerance);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[1](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[1](0, 0), -0.99f, tolerance);

    ASSERT_TRUE(std::isfinite(result.normalized_confidence_bound));
    EXPECT_NEAR(result.normalized_confidence_bound, 0.196f, tolerance);

    EXPECT_EQ(result.samples_used, total_samples);
    EXPECT_EQ(result.values_checked, 1);
    EXPECT_EQ(result.values_outside_bounds, 1);
    EXPECT_FALSE(result.passes_test);
}

TEST(InnovationAnalysisTest, PassSequence) {
    constexpr int total_samples = 100;
    constexpr int max_lag = 1;
    constexpr int num_innovations_to_skip = 0; 
    constexpr float tolerance = 0.00001f;
    std::vector<Eigen::VectorXf> innovations;

    for (int i = 0; i < total_samples; ++i) {
        innovations.push_back(Eigen::Vector<float, 1> {-1 + 2 * (i % 4 < 2)});
    }

    auto result = InnovationAnalysis::analyze(innovations, max_lag, num_innovations_to_skip);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[0](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[0](0, 0), 1.0f, tolerance);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[1](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[1](0, 0), 0.01f, tolerance);

    ASSERT_TRUE(std::isfinite(result.normalized_confidence_bound));
    EXPECT_NEAR(result.normalized_confidence_bound, 0.196f, tolerance);

    EXPECT_EQ(result.samples_used, total_samples);
    EXPECT_EQ(result.values_checked, 1);
    EXPECT_EQ(result.values_outside_bounds, 0);
    EXPECT_TRUE(result.passes_test);
}

TEST(InnovationAnalysisTest, SkipElements) {
    constexpr int total_samples = 102;
    constexpr int max_lag = 1;
    constexpr int num_innovations_to_skip = 2; 
    constexpr float tolerance = 0.00001f;
    std::vector<Eigen::VectorXf> innovations;

    innovations.push_back(Eigen::Vector<float, 1> {1e6f});
    innovations.push_back(Eigen::Vector<float, 1> {1e6f});

    for (int i = num_innovations_to_skip; i < total_samples; ++i) {
        innovations.push_back(Eigen::Vector<float, 1> {-1 + 2 * ((i - num_innovations_to_skip) % 4 < 2)});
    }

    auto result = InnovationAnalysis::analyze(innovations, max_lag, num_innovations_to_skip);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[0](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[0](0, 0), 1.0f, tolerance);

    ASSERT_TRUE(std::isfinite(result.autocorrelation_matrices[1](0, 0)));
    EXPECT_NEAR(result.autocorrelation_matrices[1](0, 0), 0.01f, tolerance);

    ASSERT_TRUE(std::isfinite(result.normalized_confidence_bound));
    EXPECT_NEAR(result.normalized_confidence_bound, 0.196f, tolerance);

    EXPECT_EQ(result.samples_used, total_samples - num_innovations_to_skip);
    EXPECT_EQ(result.values_checked, 1);
    EXPECT_EQ(result.values_outside_bounds, 0);
    EXPECT_TRUE(result.passes_test);
}

TEST(InnovationAnalysisTest, InvalidArguments) {
    constexpr int total_samples = 100;
    const std::vector<Eigen::VectorXf> innovations(total_samples, Eigen::VectorXf::Ones(1));
    const std::vector<Eigen::VectorXf> empty_innovations;

    EXPECT_THROW(InnovationAnalysis::analyze(empty_innovations, 1, 0), std::invalid_argument);

    EXPECT_THROW(InnovationAnalysis::analyze(innovations, 0, 0), std::invalid_argument);

    EXPECT_THROW(InnovationAnalysis::analyze(innovations, 1, -1), std::invalid_argument);

    EXPECT_THROW(InnovationAnalysis::analyze(innovations, 1, 50), std::invalid_argument);

    EXPECT_THROW(InnovationAnalysis::analyze(innovations, 6, 0), std::invalid_argument);
}
