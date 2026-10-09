#pragma once

class ScalarKalmanFilter {
    public:
        ScalarKalmanFilter(float x_init, float p_init, float r, float q, float sampling_time = -1.0f);

        float operator()(float z);
    private:
        float x_pred, x_post;
        float p_pred, p_post;
        float K, r, q, sampling_time;
        unsigned long timestamp_prev;
};
