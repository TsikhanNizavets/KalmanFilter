#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include "LowPassFilter.hpp"

static constexpr float dt = 0.02f;
static constexpr float total_samples = 500;

std::vector<float> generate_raw_position_data() {
    std::vector<float> data;
    data.reserve(total_samples);

    std::mt19937 gen(42);
    std::normal_distribution<float> sensor_noise(0.0f, 0.05f);

    for (int i = 0; i < total_samples; ++i) {
        float t = i * dt;

        float plate = 10.0f * std::exp(-std::pow((t - 5.0f) / 2.5f, 4.0f));
        float raw_sample = plate + sensor_noise(gen);

        data.push_back(raw_sample);
    }

    return data;
}

int main() {
    std::vector<float> raw_readings = generate_raw_position_data();
    LowPassFilter lpf(0.05f, dt);

    std::ofstream outfile(SENSOR_DATA_PATH);
    outfile << "time,raw,filtered\n";

    for (int i = 0; i < total_samples; ++i) {
        const float& raw = raw_readings[i];

        float clean = lpf(raw);

        outfile << i * dt << "," << raw << "," << clean << "\n";

    }

    return 0;
}