/**
 * @file inline.cpp
 * @author Fabian Köhler fabian.koehler@protonmail.ch
 * @version 0.1
 *
 * @section LICENSE
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Fabian Köhler
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
#include <thread>

void do_work()
{
    PERF_START();
    std::this_thread::sleep_for(perf::milliseconds(20));

    PERF_START();
    for (int i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(perf::milliseconds(20));
    }
    PERF_STOP();

    PERF_STOP();
}

int main()
{
    std::thread t0(do_work);
    std::thread t1(do_work);
    t0.join();
    t1.join();
    return 0;
}
