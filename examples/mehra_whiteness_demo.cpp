#include "data_generation/synthetic_data.hpp"
#include "innovation_analysis.hpp"
#include "linear_kalman_filter.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <Eigen/Dense>

constexpr float dt = 0.02f;
constexpr int total_samples = 4000;
constexpr float true_measurement_noise_variance = 0.0025f;
constexpr float assumed_measurement_noise_variance = 0.0025f;
constexpr float true_process_noise_variance = 33.0e-3f;
constexpr float assumed_process_noise_variance = 33.0e-1f;
constexpr int random_seed = 42;
constexpr int max_lag = 100;
constexpr int num_innovations_to_skip = 200;

int main() {
    std::vector<Reading> readings = generate_random_jerk_data(dt,total_samples, 
                                                                true_process_noise_variance, 
                                                                true_measurement_noise_variance, 
                                                                random_seed);

    // Kalman Filter Constant Acceleration 1D
    constexpr int x_dim = 3;
    constexpr int z_dim = 1;
    constexpr int u_dim = 1;

    Eigen::VectorXf x_init_vec (x_dim);
    x_init_vec << 0.0f, 0.0f, 0.0f;

    Eigen::VectorXf u(u_dim);
    u << 0.0f;

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
    
    LinearKalmanFilter kalman(x_init_vec, P_init_mat, f_function, g_function, q_function, H, R, dt);

    struct Output {
        std::vector<float> time = std::vector<float>(total_samples);
        std::vector<float> true_position = std::vector<float>(total_samples);
        std::vector<float> raw_position = std::vector<float>(total_samples);
        std::vector<float> estimated_position = std::vector<float>(total_samples);
        std::vector<Eigen::VectorXf> innovations;
        std::vector<Eigen::MatrixXf> gains;
    };
    Output output;

    for (int i = 0; i < total_samples; ++i) {
        const Reading& reading = readings[i];

        Eigen::VectorXf kalman_est = kalman(Eigen::VectorXf{{reading.raw_position}}, u);

        output.time[i] = i * dt;
        output.true_position[i] = reading.true_position;
        output.raw_position[i] = reading.raw_position;
        output.estimated_position[i] = kalman_est(0);
    }

    output.innovations = kalman.get_innovations();
    output.gains = kalman.get_gains();

    std::ofstream outfile(MEHRA_WHITENESS_DATA_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    outfile << "time,true_position,raw_position,estimated_position,innovation,pos_gain,vel_gain,acc_gain\n";
    for (int i = 0; i < total_samples; ++i) {
        outfile << output.time[i] << "," << output.true_position[i] << "," << output.raw_position[i] 
                << "," << output.estimated_position[i] << "," << output.innovations[i]
                << "," << output.gains[i](0, 0) << "," << output.gains[i](1, 0) << "," << output.gains[i](2, 0)
                << "\n";
    }
    outfile.close();

    const WhitenessResult result = InnovationAnalysis::analyze(output.innovations, max_lag, num_innovations_to_skip);

    std::cout << "Outside bounds: " << result.values_outside_bounds 
              << " / " << result.values_checked << "\n";

    std::cout << "Passes test: " << std::boolalpha << result.passes_test << "\n";

    const Eigen::MatrixXf C_0  = result.autocorrelation_matrices[0];
    const float bound = 1.96f / std::sqrt(result.samples_used);

    outfile.open(NORMALIZED_AUTOCORRELATION_DATA_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    outfile << "lag,autocorrelation,bound\n"
                "0,1," << bound << "\n";
    

    for (int i = 1; i < result.autocorrelation_matrices.size(); ++i) {
        for (int j = 0; j < result.autocorrelation_matrices[0].rows(); ++j) {
            outfile << i << "," << result.autocorrelation_matrices[i](j, j) / C_0(j, j) << "\n";
        }
    }

    return 0;
}
