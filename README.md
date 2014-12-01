cpp-perf
========

A small, header-only performance measurement library for C++11.



Easy inline code measurement
----------------------------
``` c++
// remove this #define and the code will be executed normally
#define PERF_ENABLE_INLINE
#include <perf_timer.hpp>

#include <thread>

int main() {
    // measure execution time of code and print it out
    PERF_TIME(
        std::this_thread::sleep_for(perf::milliseconds(20));
    );

    return 0;
}
```

Output:
```
/somepath/inline.cpp:8:main:   20ms
```



Automatic suite
---------------
``` c++

```



Manual suites
-------------

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
    example3 functor;

    // you can add cases via the constructor
    perf::perf_suite suite({
        // you can use lambdas
        { "example1", []() { std::this_thread::sleep_for(perf::milliseconds(42)); return true; } },
        { "example2", example2},
        { "example3", functor}
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

Output
```
===============================
Name:  perf_suite
Case        Success    Duration
-------------------------------
example1       1       42ms  
example2       1       312ms 
example3       0       147μs
example4       1       594μs
-------------------------------
Total:       0.75      355ms
===============================
```
