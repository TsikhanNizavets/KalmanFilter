#pragma once

#include <functional>
#include <eigen3/Eigen/Dense>

class KalmanFilterConstantAcceleration1D {
    public:
        using MatrixFunction = std::function<Eigen::MatrixXf(float)>;

        KalmanFilterConstantAcceleration1D(
            Eigen::VectorXf x_init, Eigen::MatrixXf P_init,
            MatrixFunction f_function, 
            MatrixFunction g_function,
            MatrixFunction q_function,
            Eigen::MatrixXf H_init, Eigen::MatrixXf R_init,
            float sampling_time = -1.0f,
            bool timeout = false);

        Eigen::VectorXf operator() (Eigen::VectorXf z, Eigen::VectorXf u);

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
        float Ts;
        unsigned long timestamp_prev;
        const Eigen::MatrixXf I;
        bool timeout;
};
