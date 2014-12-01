#include <cpp-perf.hpp>
#include <thread>

int main() {
    perf::timer t = PERF_TIMER();
    std::this_thread::sleep_for(perf::milliseconds(50));
    std::cout << t.stop() << std::endl;

    return 0;
}
