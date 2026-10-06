#pragma once

class KalmanFilter1D {
    public:
        KalmanFilter1D(float x_init, float p_init, float r, float q, float Ts = -1.0f);

        float operator()(float z);
    private:
        float x_pred, x_post;
        float p_pred, p_post;
        float Ts, K, r, q;
        unsigned long timestamp_prev;
};
