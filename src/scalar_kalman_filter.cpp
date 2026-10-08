#include "scalar_kalman_filter.hpp"
#include "utils.hpp"

KalmanFilter1D::KalmanFilter1D(float x_init, float p_init, float r_init, float q_init, float sampling_time) 
    : x_post(x_init)
    , p_post(p_init)
    , r(r_init)
    , q(q_init)
    , Ts(sampling_time)
    , timestamp_prev(utils::get_micros())
{
}

float KalmanFilter1D::operator() (float z) {
    if (Ts <= 0.0f) {
        unsigned long timestamp = utils::get_micros();

        if ((timestamp - timestamp_prev) * 1e-6f > 0.3f) {
            x_post = z;
            p_post = 1e4f;
            timestamp_prev = timestamp;
            return x_post;
        }

        timestamp_prev = timestamp;
    }

    // Step Predict
    x_pred = x_post;
    p_pred = p_post + q;

    // Step Update
    K = p_pred / (p_pred + r);
    x_post = x_pred + K * (z - x_pred);
    p_post = (1 - K) * p_pred;

    return x_post;
}
