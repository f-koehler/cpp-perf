#include <cpp-perf.hpp>
#include <thread>

int main() {
    // measure time of code betwee start and stop
    perf::timer t(perf::timer::format_name(__FILE__, __LINE__, __FUNCTION__));
    t.start();
    std::this_thread::sleep_for(perf::milliseconds(50));
    std::cout << t.stop() << std::endl;

    return 0;
}
