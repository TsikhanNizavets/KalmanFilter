#include "innovation_analysis.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

#include <Eigen/Dense>

WhitenessResult InnovationAnalysis::analyze(const std::vector<Eigen::VectorXf>& innovations, int max_lag, int num_innovations_to_skip) {
    if (innovations.empty()) {
        throw std::invalid_argument("Innovations must not be empty");
    }

    if (num_innovations_to_skip < 0 || max_lag < 1) {
        throw std::invalid_argument("Nonnegative num_innovations_to_skip and positive max_lag are expected");
    }

    const int N = innovations.size();
    const int dim = innovations[0].size();

    if ((N - num_innovations_to_skip - max_lag) < 50) {
        throw std::invalid_argument("Too few pairs at maximum lag");
    }

    if (max_lag > 0.05f * (N - num_innovations_to_skip)) {
        throw std::invalid_argument("max_lag must be much smaller than number of samples");
    }

    std::vector<Eigen::MatrixXf> C(max_lag + 1, Eigen::MatrixXf::Zero(dim, dim));

    WhitenessResult result;
    result.samples_used = N - num_innovations_to_skip;
    result.values_checked = dim * max_lag;
    result.normalized_confidence_bound = 1.96f / std::sqrt(result.samples_used);

    for (int i = 0; i < max_lag + 1; ++i) {
        for (int j = i + num_innovations_to_skip; j < N; ++j) {
            C[i] += innovations[j] * innovations[j - i].transpose();
        }
        C[i] /= (result.samples_used);
    }

    int count_out = 0;
    for (int i = 1; i < max_lag + 1; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (C[i](j, j) < -1.96f * C[0](j, j) / std::sqrt(result.samples_used) || C[i](j, j) > 1.96f * C[0](j, j) / std::sqrt(result.samples_used)) {
                count_out++;
            }
        }
    }
    
    result.autocorrelation_matrices = C;
    result.values_outside_bounds = count_out;
    result.passes_test = count_out < 0.05f * result.values_checked;

    return result;
}
