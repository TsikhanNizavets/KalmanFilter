#pragma once

class LowPassFilter {
    public:
        // Constructor: Tf = time constant, Ts = optional fixed sampling time
        LowPassFilter(float Tf, float Ts = -1.0f, float initial_y = 0.0f);

        // Filter computation method
        float operator()(float x);

        void reset(float y_init);

    private:
        float Tf; // Time con   stant (seconds)
        float Ts; // Fixed sampling period (seconds)
        unsigned long timestamp_prev; // Microseconds timestamp of last execution
        float y_prev;                 // Output value from previous iteration
};
