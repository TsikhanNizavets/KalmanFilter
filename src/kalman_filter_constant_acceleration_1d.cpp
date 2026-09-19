#include <functional>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include "utils.hpp"
#include "kalman_filter_constant_acceleration_1d.hpp"

KalmanFilterConstantAcceleration1D::KalmanFilterConstantAcceleration1D(
    Eigen::VectorXf x_init, Eigen::MatrixXf P_init,
    MatrixFunction f_function, 
    MatrixFunction g_function, 
    MatrixFunction q_function,
    Eigen::MatrixXf H_init, Eigen::MatrixXf R_init,
    float sampling_time
)
    : x_pred(std::move(x_init))
    , P_pred(std::move(P_init))
    , f_function(std::move(f_function))
    , g_function(std::move(g_function))
    , q_function(std::move(q_function))
    , H(std::move(H_init))
    , R(std::move(R_init))
    , Ts(sampling_time)
    , timestamp_prev(utils::get_micros())
    , I(Eigen::MatrixXf::Identity(P_pred.rows(), P_pred.cols()))
{
}

Eigen::VectorXf KalmanFilterConstantAcceleration1D::operator() (Eigen::VectorXf z, Eigen::VectorXf u) {
    float dt = Ts;
    
    if (dt <= 0.0f) {
        unsigned long timestamp = utils::get_micros();
        dt = (timestamp - timestamp_prev) * 1e-6f;

        if (dt > 0.3f) {
            P_pred = 1e4f * I;
        }

        timestamp_prev = timestamp;
    }

    const Eigen::MatrixXf F = f_function(dt);
    const Eigen::MatrixXf G = g_function(dt);
    const Eigen::MatrixXf Q = q_function(dt);

    // Step Update
    K = P_pred * H.transpose() * (H * P_pred * H.transpose() + R).inverse();
    x_post = x_pred + K * (z - H * x_pred);
    P_post = (I - K * H) * P_pred * (I - K * H).transpose() + K * R * K.transpose();

    // Step Predict
    x_pred = F * x_post + G * u;
    P_pred = F * P_post * F.transpose() + Q;

    return x_post;
}
