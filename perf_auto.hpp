#ifndef PERF_AUTO_HPP
#define PERF_AUTO_HPP

#include <iostream>
#include "perf_suite.hpp"

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
