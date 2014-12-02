#include <cpp-perf.hpp>
PERF_BEGIN("ExampleModule")

PERF_CASE("example1",
    std::this_thread::sleep_for(perf::milliseconds(20));
)

PERF_CASE("example1",
    for(int i = 0; i < 20; i++) std::this_thread::sleep_for(perf::milliseconds(30));
)

PERF_END()
