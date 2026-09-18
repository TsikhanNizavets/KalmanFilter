#include "kalman_filter_1d.hpp"
#include "utils.hpp"

KalmanFilter1D::KalmanFilter1D(float x_init, float p_init, float r, float q, float sampling_time) 
    : x_pred(x_init)
    , p_pred(p_init)
    , r_(r)
    , q_(q)
    , Ts(sampling_time)
    , timestamp_prev(utils::get_micros())
{
}

float KalmanFilter1D::operator() (float z) {
    float dt = Ts;
    
    if (dt <= 0.0f) {
        unsigned long timestamp = utils::get_micros();
        dt = (timestamp - timestamp_prev) * 1e-6f;

        if (dt > 0.3f) {
            x_pred = z;
            timestamp_prev = timestamp;
            return x_pred;
        }

        timestamp_prev = timestamp;
    }

    // Step Update
    K = p_pred / (p_pred + r_);
    x_post = x_pred + K * (z - x_pred);
    p_post = (1 - K) * p_pred;

    // Step Predict
    x_pred = x_post;
    p_pred = p_post + q_;

    return x_post;
}
