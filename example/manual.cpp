/**
 * @file manual.cpp
 * @author Fabian Köhler fabian2804@googlemail.com
 * @version 0.1
 *
 * @section LICENSE
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Fabian Köhler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


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
