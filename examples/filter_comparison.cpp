#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "low_pass_filter.hpp"
#include "scalar_kalman_filter.hpp"
#include "linear_kalman_filter.hpp"
#include "data_generation/synthetic_data.hpp"

constexpr float dt = 0.02f;
constexpr int total_samples = 500;
constexpr float true_measurement_noise_variance = 0.0025f;
constexpr float assumed_measurement_noise_variance = 0.0025f;
constexpr float true_process_noise_variance = 33.0e-3f;
constexpr float assumed_process_noise_variance = 33.0e-0f;
constexpr int random_seed = 42;

int main() {
    float Ts = dt;
    std::vector<Reading> readings = generate_bump_trajectory_data(dt,total_samples, 
                                                                    true_process_noise_variance, 
                                                                    true_measurement_noise_variance, 
                                                                    random_seed);

    // Low-Pass Filter
    float Tf = 0.05f;
    LowPassFilter lpf(Tf, Ts);

    // Kalman Filter 1D
    float x_init = 0.0f;
    float p_init = 0.1f;
    float r = 0.0025f;
    float q = 0.00033f;
    KalmanFilter1D kf1d(x_init, p_init, r, q, Ts);

    // Kalman Filter Constant Acceleration 1D
    int x_dim = 3;
    int z_dim = 1;
    int u_dim = 1;

    Eigen::VectorXf x_init_vec (x_dim);
    x_init_vec << 0.0f, 0.0f, 0.0f;

    Eigen::MatrixXf P_init_mat = Eigen::MatrixXf::Zero(x_dim, x_dim);
    P_init_mat.diagonal() << 1.0f, 1.0f, 1.0f;

    auto f_function = [x_dim](float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf F(x_dim, x_dim);

        F << 1.0f, dt, dt * dt / 2,
             0, 1, dt,
             0, 0, 1;

        return F;
    };

    auto g_function = [x_dim, u_dim](float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf G(x_dim, u_dim);

        G << 0.0f, 0.0f, 0.0f;

        return G;
    };

    auto q_function = [x_dim](float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf Q(x_dim, x_dim);

        float dt2 = dt * dt;
        float dt3 = dt * dt * dt;
        float dt4 = dt2 * dt2;
        float dt5 = dt3 * dt2;
        float dt6 = dt3 * dt3;

        Q << dt6 / 36.0f, dt5 / 12.0f, dt4 / 6.0f,
            dt5 / 12.0f, dt4 / 4.0f, dt3 / 2.0f,
            dt4 / 6.0f, dt3 / 2.0f, dt2;

        return Q * assumed_process_noise_variance;
    };

    Eigen::MatrixXf H (z_dim, x_dim);
    H << 1.0f, 0.0f, 0.0f;

    Eigen::MatrixXf R (z_dim, z_dim);
    R << assumed_measurement_noise_variance;
    
    KalmanFilterConstantAcceleration1D kfca1d(x_init_vec, P_init_mat, f_function, g_function, q_function, H, R, Ts);

    std::ofstream outfile(FILTER_COMPARISON_DATA_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }
    outfile << "time,true_position,raw_position,low_pass,kalman,kalman_const_acc\n";

    for (int i = 0; i < total_samples; ++i) {
        const Reading& reading = readings[i];

        float low_pass = lpf(reading.raw_position);
        float kalman1d = kf1d(reading.raw_position);
        Eigen::VectorXf kalmanca1d = kfca1d(Eigen::VectorXf{{reading.raw_position}}, Eigen::VectorXf{{0.0f}});

        outfile << i * dt << "," << reading.true_position << "," << reading.raw_position << "," 
            << low_pass << "," << kalman1d << "," << kalmanca1d(0) << "\n";
    }

    return 0;
}