#pragma once

#include <eigen3/Eigen/Dense>
#include <vector>

struct WhitenessResult {
    std::vector<Eigen::MatrixXf> autocorrelation_matrices;

    int samples_used = 0;
    int values_checked = 0;
    int values_outside_bounds = 0;

    float normalized_confidence_bound = 0.0f;
    bool passes_test = false;
};

class InnovationAnalysis {
    public:
        InnovationAnalysis();

        static WhitenessResult analyze(
            const std::vector<Eigen::VectorXf>& innovations, 
            int k, 
            int num_innovations_to_skip);
            
    private:


};
