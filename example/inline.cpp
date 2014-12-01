#define PERF_ENABLE_INLINE
#include <perf_timer.hpp>

#include <thread>

int main() {
    PERF_TIME(
        std::this_thread::sleep_for(perf::milliseconds(20));
    );

    return 0;
}
