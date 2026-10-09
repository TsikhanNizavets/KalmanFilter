#include "synthetic_data.hpp"

#include <cmath>
#include <random>

std::vector<Reading> generate_bump_trajectory_data(float dt, int total_samples, 
                                                        float true_process_noise_variance, 
                                                        float true_measurement_noise_variance, 
                                                        int random_seed) 
{
    const float dt2 = dt * dt;
    const float dt3 = dt * dt * dt;

    std::vector<Reading> data(total_samples, Reading{0.0f, 0.0f});

    std::mt19937 gen(random_seed);
    std::normal_distribution<float> process_noise(0.0f, std::sqrt(true_process_noise_variance));
    std::normal_distribution<float> measurement_noise(0.0f, std::sqrt(true_measurement_noise_variance));

    float pos_err = 0.0f;
    float vel_err = 0.0f;
    float acc_err = 0.0f;

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
        
        float u = (time - 5.0f) / 2.5f;
        float u4 = u * u * u * u;

        float true_position_sample = 10.0f * std::exp(-u4) + pos_err;
        float raw_position_sample = true_position_sample + measurement_noise(gen);

        data[i].true_position = true_position_sample;
        data[i].raw_position = raw_position_sample;
    }

    return data;
}

std::vector<Reading> generate_random_jerk_data(float dt, int total_samples, 
                                                float true_process_noise_variance, 
                                                float true_measurement_noise_variance, 
                                                int random_seed)
{
    const float dt2 = dt * dt;
    const float dt3 = dt * dt * dt;

    std::vector<Reading> data(total_samples, Reading{0.0f, 0.0f});

    std::mt19937 gen(random_seed);
    std::normal_distribution<float> process_noise(0.0f, std::sqrt(true_process_noise_variance));
    std::normal_distribution<float> measurement_noise(0.0f, std::sqrt(true_measurement_noise_variance));

    float pos_err = 0.0f;
    float vel_err = 0.0f;
    float acc_err = 0.0f;
    constexpr float vel = 0.5f;

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
        float raw_position_sample = true_position_sample + measurement_noise(gen);

        data[i].true_position = true_position_sample;
        data[i].raw_position = raw_position_sample;
    }

    return data;
}
