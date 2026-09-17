#include <chrono>
#include "LowPassFilter.hpp"

static unsigned long get_micros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

LowPassFilter::LowPassFilter(
    float time_constant,
    float sampling_time,
    float y_init
) 
    : Tf(time_constant)
    , Ts(sampling_time)
    , timestamp_prev(get_micros())
    , y_prev(y_init)
{
}

float LowPassFilter::operator() (float x) {
    float dt = Ts;

    if (dt <= 0.0f) {
        unsigned long timestamp = get_micros();
        dt = (timestamp - timestamp_prev) * 1e-6f;

        if (dt <= 0.0f || dt > 0.3f) {
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
    timestamp_prev = get_micros();
}
