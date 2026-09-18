#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include "low_pass_filter.hpp"
#include "kalman_filter_1d.hpp"

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
    std::vector<Reading> readings = generate_raw_position_data();

    float Tf = 0.05f;
    float Ts = dt;
    LowPassFilter lpf(Tf, Ts);

    float x_pred = 0.0f;
    float p_pred = 0.1f;
    float r = 0.0025f;
    float q = 0.00033f;
    KalmanFilter1D kf1d(x_pred, p_pred, r, q, Ts);

    std::ofstream outfile(SENSOR_DATA_PATH);
    if (!outfile) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }
    outfile << "time,true_position,raw,low_pass,kalman\n";

    for (int i = 0; i < total_samples; ++i) {
        const Reading& reading = readings[i];

        float low_pass = lpf(reading.raw);
        float kalman1d = kf1d(reading.raw);

        outfile << i * dt << "," << reading.true_position << "," << reading.raw << "," << low_pass << "," << kalman1d << "\n";
    }

    return 0;
}