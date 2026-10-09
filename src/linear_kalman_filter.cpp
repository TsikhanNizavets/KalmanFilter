#include "linear_kalman_filter.hpp"
#include "utils.hpp"

#include <utility>

#include <Eigen/Dense>

LinearKalmanFilter::LinearKalmanFilter(
    Eigen::VectorXf x_init, Eigen::MatrixXf P_init,
    MatrixFunction f_function, 
    MatrixFunction g_function, 
    MatrixFunction q_function,
    Eigen::MatrixXf H_init, Eigen::MatrixXf R_init,
    float sampling_time,
    bool reset_covariance_on_timeout
)
    : x_post(std::move(x_init))
    , P_post(std::move(P_init))
    , f_function(std::move(f_function))
    , g_function(std::move(g_function))
    , q_function(std::move(q_function))
    , H(std::move(H_init))
    , R(std::move(R_init))
    , sampling_time(sampling_time)
    , timestamp_prev(utils::get_micros())
    , I(Eigen::MatrixXf::Identity(P_post.rows(), P_post.cols()))
    , reset_covariance_on_timeout(reset_covariance_on_timeout)
{
}

Eigen::VectorXf LinearKalmanFilter::operator() (Eigen::VectorXf z, Eigen::VectorXf u) {
    float dt = sampling_time;
    
    if (dt <= 0.0f) {
        unsigned long timestamp = utils::get_micros();
        dt = (timestamp - timestamp_prev) * 1e-6f;

        if (dt > 0.3f && reset_covariance_on_timeout) {
            P_post = 1e4f * I;
        }

        timestamp_prev = timestamp;
    }

    const Eigen::MatrixXf F = f_function(dt);
    const Eigen::MatrixXf G = g_function(dt);
    const Eigen::MatrixXf Q = q_function(dt);

    // Step Predict
    x_pred = F * x_post + G * u;
    P_pred = F * P_post * F.transpose() + Q;

    // Step Update
    K = P_pred * H.transpose() * (H * P_pred * H.transpose() + R).inverse();
    x_post = x_pred + K * (z - H * x_pred);
    P_post = (I - K * H) * P_pred * (I - K * H).transpose() + K * R * K.transpose();

    const Eigen::VectorXf innovation = z - H * x_pred;
    innovations.push_back(innovation);
    gains.push_back(K);

    return x_post;
}

std::vector<Eigen::VectorXf> LinearKalmanFilter::get_innovations() {
    return innovations;
}

std::vector<Eigen::MatrixXf> LinearKalmanFilter::get_gains() {
    return gains;
}
