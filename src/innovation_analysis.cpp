#include <eigen3/Eigen/Dense>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "innovation_analysis.hpp"

InnovationAnalysis::InnovationAnalysis() {}

WhitenessResult InnovationAnalysis::analyze(const std::vector<Eigen::VectorXf>& innovations, int k, int num_innovations_to_skip) {
    if (innovations.empty()) {
        throw std::invalid_argument("Innovations must not be empty");
    }

    if (num_innovations_to_skip < 0 || k < 1) {
        throw std::invalid_argument("Nonnegative num_innovations_to_skip and positive k are expected");
    }

    const int N = innovations.size();
    const int dim = innovations[0].size();

    if ((N - num_innovations_to_skip - k) < 50) {
        throw std::invalid_argument("Too few pairs at maximum lag");
    }

    if (k > 0.05f * (N - num_innovations_to_skip)) {
        throw std::invalid_argument("K must be much smaller than number of samples");
    }

    std::vector<Eigen::MatrixXf> C(k + 1, Eigen::MatrixXf::Zero(dim, dim));

    for (int i = 0; i < k + 1; ++i) {
        for (int j = i + num_innovations_to_skip; j < N; ++j) {
            C[i] += innovations[j] * innovations[j - i].transpose();
        }
        C[i] /= (N - num_innovations_to_skip);
    }

    int count_out = 0;
    for (int i = 1; i < k + 1; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (C[i](j, j) < -1.96f * C[0](j, j) / std::sqrt(N - num_innovations_to_skip) || C[i](j, j) > 1.96f * C[0](j, j) / std::sqrt(N - num_innovations_to_skip)) {
                count_out++;
            }
        }
    }
    
    WhitenessResult result;

    result.autocorrelation_matrices = C;
    result.samples_used = N - num_innovations_to_skip;  // use smaples_used by calculation of C[i]
    result.values_checked = dim * k;
    result.values_outside_bounds = count_out;

    result.normalized_confidence_bound = 1.96f / std::sqrt(result.samples_used);

    result.passes_test = count_out < 0.05f * result.values_checked;

    return result;
}