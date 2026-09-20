#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "low_pass_filter.hpp"
#include "kalman_filter_1d.hpp"
#include "kalman_filter_constant_acceleration_1d.hpp"

static constexpr float dt = 0.02f;
static constexpr int total_samples = 500;

struct Reading {
    float true_position;
    float raw;
};

std::vector<Reading> generate_raw_position_data() {
    std::vector<Reading> data(total_samples, Reading{0.0f, 0.0f});

    std::mt19937 gen(42);
    std::normal_distribution<float> sensor_noise(0.0f, 0.05f);

    for (int i = 0; i < total_samples; ++i) {
        float time = i * dt;

        float plate = 10.0f * std::exp(-std::pow((time - 5.0f) / 2.5f, 4.0f));
        float raw_sample = plate + sensor_noise(gen);

        data[i].true_position = plate;
        data[i].raw = raw_sample;
    }

    return data;
}

int main() {
    float Ts = dt;
    std::vector<Reading> readings = generate_raw_position_data();

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
    float process_noise_factor = 33e-3f;

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

    auto q_function = [x_dim, process_noise_factor](float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf Q(x_dim, x_dim);

        Q << dt * dt * dt * dt / 4.0f,
            dt * dt * dt / 2.0f,
            dt * dt / 2.0f,

            dt * dt * dt / 2.0f,
            dt * dt,
            dt,

            dt * dt / 2.0f,
            dt,
            1.0f;

        return Q * process_noise_factor;
    };

    Eigen::MatrixXf H (z_dim, x_dim);
    H << 1.0f, 0.0f, 0.0f;

    Eigen::MatrixXf R (z_dim, z_dim);
    R << 0.0025f;
    
    KalmanFilterConstantAcceleration1D kfca1d(x_init_vec, P_init_mat, f_function, g_function, q_function, H, R, Ts);

    std::ofstream outfile(SENSOR_DATA_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }
    outfile << "time,true_position,raw,low_pass,kalman,kalman_const_acc\n";

    for (int i = 0; i < total_samples; ++i) {
        const Reading& reading = readings[i];

        float low_pass = lpf(reading.raw);
        float kalman1d = kf1d(reading.raw);
        Eigen::VectorXf kalmanca1d = kfca1d(Eigen::VectorXf{{reading.raw}}, Eigen::VectorXf{{0.0f}});

        outfile << i * dt << "," << reading.true_position << "," << reading.raw << "," 
            << low_pass << "," << kalman1d << "," << kalmanca1d(0) << "\n";
    }

    return 0;
}