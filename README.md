cpp-perf
========

A small, header-only performance measurement library for C++11.



Easy inline code measurement
----------------------------
``` c++
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
```

Output:
```
```



Automatic suite
---------------
``` c++

```



Manual suites
-------------

``` c++
#include <cpp-perf.hpp>
#include <thread>
using namespace std;

// you can use functions
bool example2() {
    std::this_thread::sleep_for(perf::milliseconds(100));
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
    perf::suite suite({
        // you can use lambdas
        { "example1", []() { std::this_thread::sleep_for(perf::milliseconds(40)); return true; } },
        { "example2", example2},
        { "example3", functor}
    });

    // you can add cases later
    suite.add_case("example4", []() { std::this_thread::sleep_for(perf::microseconds(800)); return true; });

    // run all cases at once
    suite.run();

    // print results
    cout << suite << endl;

    return 0;
}
```

Output:
```
===============================
Name:  PerfSuite
Case        Success    Duration
-------------------------------
example1       1       40ms  
example2       1       100ms 
example3       0       70μs 
example4       1       869μs
-------------------------------
Total:       0.75      141ms
===============================
```
