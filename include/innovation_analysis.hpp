#pragma once

#include <eigen3/Eigen/Dense>

class InnovationAnalysis {
    public:
        InnovationAnalysis();

        static bool areInnovationsWhite(std::vector<Eigen::VectorXf> innovations, int k, int num_innovations_to_skip);
    private:


};