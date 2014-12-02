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
