cpp-perf
========

A small, header-only performance measurement library for C++11.


Most basic usage
----------------
``` c++
#include <cpp-perf.hpp>

int main()
{
    perf::start();
    std::this_thread::sleep_for(perf::milliseconds(20));
    perf::stop();

    return 0;
}
```

Output:
```
20 ms
```

Inline measurement
------------------
``` c++
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
```

Output:
```
/somepath/perf/example/inline.cpp:main:7-11: 200ms
/somepath/perf/example/inline.cpp:main:4-13: 220ms
```
If you put ```#ifdef PERF_DISABLE_INLINE``` in front of the include ```#include <cpp-perf.hpp>``` there will no measurement be performed. The macros are actually expanded int empty lines so that your code is the same as if you would not have put them there.


Automatic suite
---------------

``` c++
#include <cpp-perf.hpp>
PERF_BEGIN("ExampleModule")

PERF_CASE("example1",
    std::this_thread::sleep_for(perf::milliseconds(20));
)

PERF_CASE("example1",
    for(int i = 0; i < 20; i++) std::this_thread::sleep_for(perf::milliseconds(30));
)

PERF_END()
```

Output:
```
1/2 Case #1: example1 .....................  Passed  20 ms
2/2 Case #2: example2 .....................  Passed  601 ms
```


Manual suites
-------------

``` c++
#include <cpp-perf.hpp>

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
    std::cout << suite << std::endl;

    return 0;
}
```

Output:
```
1/4 Case #1: example1 .....................  Passed  40 ms
2/4 Case #2: example2 .....................  Passed  100 ms
3/4 Case #3: example3 .....................  Failed  61 μs
4/4 Case #4: example4 .....................  Passed  859 μs
```
