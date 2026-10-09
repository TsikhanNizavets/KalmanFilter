#pragma once

class LowPassFilter {
    public:
        // Constructor: time_constant = time constant, sampling_time = optional fixed sampling time
        LowPassFilter(float time_constant, float sampling_time = -1.0f, float initial_y = 0.0f);

        // Filter computation method
        float operator()(float x);

        void reset(float y_init);

    private:
        float time_constant; // Time constant (seconds)
        float sampling_time; // Fixed sampling period (seconds)
        unsigned long timestamp_prev; // Microseconds timestamp of last execution
        float y_prev;                 // Output value from previous iteration
};
