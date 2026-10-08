#include <eigen3/Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <gtest/gtest.h>
#include <unordered_map>
#include "linear_kalman_filter.hpp"

TEST(LinearKalmanFilterTest, 2DTracking) {
    static constexpr float dt = 1.0f;
    const std::vector<Eigen::Vector2f> measurements = {
        Eigen::Vector2f(301.50f, -401.46f),
        Eigen::Vector2f(298.23f, -375.44f),
        Eigen::Vector2f(297.83f, -346.15f),
        Eigen::Vector2f(300.42f, -320.20f),
        Eigen::Vector2f(301.94f, -300.08f),
        Eigen::Vector2f(299.50f, -274.12f),
        Eigen::Vector2f(305.98f, -253.45f),
        Eigen::Vector2f(301.25f, -226.40f),
        Eigen::Vector2f(299.73f, -200.65f),
        Eigen::Vector2f(299.20f, -171.62f),
        Eigen::Vector2f(298.62f, -152.11f),
        Eigen::Vector2f(301.84f, -125.19f),
        Eigen::Vector2f(299.60f,  -93.40f),
        Eigen::Vector2f(295.30f,  -74.79f),
        Eigen::Vector2f(299.30f,  -49.12f),
        Eigen::Vector2f(301.95f,  -28.73f),
        Eigen::Vector2f(296.30f,    2.99f),
        Eigen::Vector2f(295.11f,   25.65f),
        Eigen::Vector2f(295.12f,   49.86f),
        Eigen::Vector2f(289.90f,   72.87f),
        Eigen::Vector2f(283.51f,   96.34f),
        Eigen::Vector2f(276.42f,  120.40f),
        Eigen::Vector2f(264.22f,  144.69f),
        Eigen::Vector2f(250.25f,  168.06f),
        Eigen::Vector2f(236.66f,  184.99f),
        Eigen::Vector2f(217.47f,  205.11f),
        Eigen::Vector2f(199.75f,  221.82f),
        Eigen::Vector2f(179.70f,  238.30f),
        Eigen::Vector2f(160.00f,  253.02f),
        Eigen::Vector2f(140.92f,  267.19f),
        Eigen::Vector2f(113.53f,  270.71f),
        Eigen::Vector2f( 93.68f,  285.86f),
        Eigen::Vector2f( 69.71f,  288.48f),
        Eigen::Vector2f( 45.93f,  292.90f),
        Eigen::Vector2f( 20.87f,  298.77f)
    };

    constexpr int x_dim = 6;
    constexpr int z_dim = 2;
    constexpr int u_dim = 1;
    constexpr float process_noise_variance = 0.04;

    Eigen::VectorXf x_init (x_dim);
    x_init << 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f;

    Eigen::VectorXf u(u_dim);
    u << 0.0f;

    Eigen::MatrixXf P_init = Eigen::MatrixXf::Zero(x_dim, x_dim);
    P_init.diagonal() << 500.0f, 500.0f, 500.0f, 500.0f, 500.0f, 500.0f;

    auto f_function = [x_dim] (float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf F(x_dim, x_dim);

        F << 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f,
             0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f;

        return F;
    };

    auto g_function = [x_dim, u_dim] (float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf G(x_dim, u_dim);

        G << 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f;

        return G;
    };

    auto q_function = [x_dim, process_noise_variance] (float dt) -> Eigen::MatrixXf {
        Eigen::MatrixXf Q(x_dim, x_dim);

        Q << 0.25f, 0.5f, 0.5f, 0.0f,  0.0f, 0.0f,
             0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.0f,  0.0f, 0.0f, 0.25f, 0.5f, 0.5f,
             0.0f,  0.0f, 0.0f, 0.5f,  1.0f, 1.0f,
             0.0f,  0.0f, 0.0f, 0.5f,  1.0f, 1.0f;

        return Q * process_noise_variance;
    };

    Eigen::MatrixXf H_init (z_dim, x_dim);
    H_init << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f;

    Eigen::MatrixXf R_init (z_dim, z_dim);
    R_init << 9.0f, 0.0f,
              0.0f, 9.0f;

    auto kalman = KalmanFilterConstantAcceleration1D(x_init, P_init, f_function, g_function, q_function, H_init, R_init, dt);

    std::unordered_map<int, Eigen::Vector<float, x_dim>> x_est_test;
    x_est_test[0] << 299.1f, 199.4f, 66.47f, -398.27f, -265.52f, -88.51f;
    x_est_test[1] << 300.37f, -28.22f, -66.53f, -378.49f, 64.87f, 100.93f;
    x_est_test[34] << 19.3f, -25.99f, -0.74f, 297.79f, 2.03f, -1.86f;

    constexpr float tolerance = 0.01f;

    for (int i = 0; i < 35; ++i) {
        Eigen::VectorXf x_est = kalman(measurements[i], u);
        
        if (i == 0 || i == 1 || i == 34) {
            ASSERT_TRUE(x_est.allFinite());
            for (int j = 0; j < x_dim; ++j) {
                EXPECT_NEAR(x_est[j], x_est_test[i][j], tolerance);
            }
        }
    }
};
