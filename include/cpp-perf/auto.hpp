#ifndef AUTO_HPP
#define AUTO_HPP

#include <cpp-perf/suite.hpp>
#include <iostream>

#ifndef PERF_AUTO_MODULE
#define PERF_AUTO_MODULE "PerfAutoModule"
#endif

perf::perf_suite auto_suite("Module");

#define PERF_ADD_FUNCTION(n, f) auto_suite.add_case(n, f);

int main() {
    auto_suite.run();
    return 0;
}

#endif
