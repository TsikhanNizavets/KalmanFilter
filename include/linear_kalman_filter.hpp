#pragma once

#include <functional>
#include <vector>

#include <Eigen/Dense>

class LinearKalmanFilter {
    public:
        using MatrixFunction = std::function<Eigen::MatrixXf(float)>;

        LinearKalmanFilter(
            Eigen::VectorXf x_init, Eigen::MatrixXf P_init,
            MatrixFunction f_function, 
            MatrixFunction g_function,
            MatrixFunction q_function,
            Eigen::MatrixXf H_init, Eigen::MatrixXf R_init,
            float sampling_time = -1.0f,
            // Reset covariance after a long pause when using automatic timing.
            bool reset_covariance_on_timeout = false);

        Eigen::VectorXf operator() (Eigen::VectorXf z, Eigen::VectorXf u);

        std::vector<Eigen::VectorXf> get_innovations ();

        std::vector<Eigen::MatrixXf> get_gains ();

    private:
        // Updated during filtering
        Eigen::VectorXf x_pred, x_post;
        Eigen::MatrixXf P_pred, P_post, K;

        // System and measurement models
        MatrixFunction f_function;
        MatrixFunction g_function;
        MatrixFunction q_function;
        Eigen::MatrixXf H, R;

        //Other
        float sampling_time;
        unsigned long timestamp_prev;
        const Eigen::MatrixXf I;
        bool reset_covariance_on_timeout;
        std::vector<Eigen::VectorXf> innovations;
        std::vector<Eigen::MatrixXf> gains;
};
