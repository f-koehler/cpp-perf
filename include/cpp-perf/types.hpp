#ifndef TYPES_HPP
#define TYPES_HPP

#include <chrono>
#include <functional>

namespace perf
{
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point time_point;
    typedef clock::duration duration;
    typedef std::chrono::hours hours;
    typedef std::chrono::minutes minutes;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::nanoseconds nanoseconds;

    typedef std::function<bool(void)> perf_func;

    struct perf_case {
        perf_func f;
        std::string name;
        bool success;
        duration time;
    };


}

#endif
