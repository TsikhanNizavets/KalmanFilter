#pragma once

#include <chrono>

namespace utils {
    inline unsigned long get_micros() {
        using namespace std::chrono;
        return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    }
};
