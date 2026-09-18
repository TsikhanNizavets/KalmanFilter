#include "LowPassFilter.hpp"
#include "utils.hpp"

LowPassFilter::LowPassFilter(
    float time_constant,
    float sampling_time,
    float y_init
) 
    : Tf(time_constant)
    , Ts(sampling_time)
    , timestamp_prev(utils::get_micros())
    , y_prev(y_init)
{
}

float LowPassFilter::operator() (float x) {
    float dt = Ts;

    if (dt <= 0.0f) {
        unsigned long timestamp = utils::get_micros();
        dt = (timestamp - timestamp_prev) * 1e-6f;

        if (dt > 0.3f) {
            y_prev = x;
            timestamp_prev = timestamp;
            return x;
        }
        timestamp_prev = timestamp;
    }

    float alpha = Tf / (Tf + dt);
    float y = alpha * y_prev + (1 - alpha) * x;

    y_prev = y;
    return y;
}

void LowPassFilter::reset(float y_init) {
    y_prev = y_init;
    timestamp_prev = utils::get_micros();
}
