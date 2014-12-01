perf
====

A small, header-only performance measurement library for C++11.

Automatic execution
-------------------
``` c++

```

Manual execution
----------------

``` c++
#include <perf_suite.hpp>
#include <thread>
#include <iostream>
using namespace std;

// you can use functions
bool example2() {
    std::this_thread::sleep_for(perf::milliseconds(312));
    return true;
}

// you can use functors
struct example3 {
    bool operator()() {
        // exceptions will be caught
        throw std::string("This fails!");
        return true;
    }
};

int main()
{
    // you can add cases via the constructor
    perf::perf_suite suite({
        // you can use lambdas
        { "example1", []() { std::this_thread::sleep_for(perf::milliseconds(42)); return true; } },
        { "example2", example2},
        { "example3", example3()}
    });

    // you can add cases later
    suite.add_case("example4", []() { std::this_thread::sleep_for(perf::microseconds(512)); return true; });

    // run all cases at once
    suite.run();

    // print results
    cout << suite << endl;

    return 0;
}
```
