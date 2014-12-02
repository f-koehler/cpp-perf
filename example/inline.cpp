#include <cpp-perf.hpp>

int main() {
    PERF_START();
    std::this_thread::sleep_for(perf::milliseconds(20));

    PERF_START();
    for(int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(perf::milliseconds(20));
    }
    PERF_STOP();

    PERF_STOP();

    return 0;
}
