#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "low_pass_filter.hpp"
#include "kalman_filter_1d.hpp"
#include "kalman_filter_constant_acceleration_1d.hpp"
#include "innovation_analysis.hpp"

static constexpr float dt = 0.02f;
static constexpr int total_samples = 5000;
static constexpr float sensor_noise_variance = 0.0025f;
constexpr float simulation_jerk_variance = 33.0e-3f;
constexpr float filter_jerk_variance = 33.0e-1f;

static constexpr float dt2 = dt * dt;
static constexpr float dt3 = dt * dt * dt;

struct Reading {
    float true_position;
    float raw_position;
};

std::vector<Reading> generate_raw_position_data() {
    std::vector<Reading> data(total_samples, Reading{0.0f, 0.0f});

    std::mt19937 gen(42);
    std::normal_distribution<float> sensor_noise(0.0f, std::sqrt(sensor_noise_variance));
    std::normal_distribution<float> process_noise(0.0f, std::sqrt(simulation_jerk_variance));

    float pos_err = 0.0f;
    float vel_err = 0.0f;
    float acc_err = 0.0f;
    float vel = 0.5f;

    for (int i = 0; i < total_samples; ++i) {
        float time = i * dt;

        if (i > 0) {
            float j = process_noise(gen);
            pos_err += vel_err * dt
                    + acc_err * dt2 / 2
                    + j * dt3 / 6.0f;
            vel_err += acc_err * dt
                    + j * dt2 / 2.0f;
            acc_err += j * dt;
        }

        float true_position_sample = vel * time + pos_err;
        float raw_position_sample = true_position_sample + sensor_noise(gen);

        data[i].true_position = true_position_sample;
        data[i].raw_position = raw_position_sample;
    }

    return data;
}

int main() {
    float Ts = dt;
    std::vector<Reading> readings = generate_raw_position_data();

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

        return Q * filter_jerk_variance;
    };

    Eigen::MatrixXf H (z_dim, x_dim);
    H << 1.0f, 0.0f, 0.0f;

    Eigen::MatrixXf R (z_dim, z_dim);
    R << 0.0025f;
    
    KalmanFilterConstantAcceleration1D kalman(x_init_vec, P_init_mat, f_function, g_function, q_function, H, R, Ts);

    struct Output {
        std::vector<float> time = std::vector<float>(total_samples);
        std::vector<float> true_position = std::vector<float>(total_samples);
        std::vector<float> raw_position = std::vector<float>(total_samples);
        std::vector<float> estimated_position = std::vector<float>(total_samples);
        std::vector<Eigen::VectorXf> innovation;
        std::vector<Eigen::MatrixXf> gain;
    };
    Output output;

    for (int i = 0; i < total_samples; ++i) {
        const Reading& reading = readings[i];

        Eigen::VectorXf kalman_est = kalman(Eigen::VectorXf{{reading.raw_position}}, Eigen::VectorXf{{0.0f}});

        output.time[i] = i * dt;
        output.true_position[i] = reading.true_position;
        output.raw_position[i] = reading.raw_position;
        output.estimated_position[i] = kalman_est(0);
    }

    output.innovation = kalman.get_innovations();
    output.gain = kalman.get_gains();

    std::ofstream outfile(MEHRA_TEST_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    outfile << "time,true_position,raw_position,estimated_position,innovation,pos_gain,vel_gain,acc_gain\n";
    for (int i = 0; i < total_samples; ++i) {
        outfile << output.time[i] << "," << output.true_position[i] << "," << output.raw_position[i] 
                << "," << output.estimated_position[i] << "," << output.innovation[i] 
                << "," << output.gain[i](0, 0) << "," << output.gain[i](1, 0) << "," << output.gain[i](2, 0)
                << "\n";
    }

    std::cout << InnovationAnalysis::areInnovationsWhite(output.innovation, 100, 200) << "\n";

    return 0;
}
