#pragma once

#include <vector>

struct Reading {
    float true_position;
    float raw_position;
};

std::vector<Reading> generate_bump_trajectory_data(float dt, int total_samples, 
                                                    float true_process_noise_variance, 
                                                    float true_measurement_noise_variance, 
                                                    int random_seed);

std::vector<Reading> generate_random_jerk_data(float dt, int total_samples, 
                                                float true_process_noise_variance, 
                                                float true_measurement_noise_variance, 
                                                int random_seed);
