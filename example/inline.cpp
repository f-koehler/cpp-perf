#include <cpp-perf.hpp>
#include <thread>

int main() {
    PERF_START();
    std::this_thread::sleep_for(perf::milliseconds(20));
    PERF_STOP();

    return 0;
}
