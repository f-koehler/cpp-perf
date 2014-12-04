/**
 * @file cpp-perf.hpp
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

#ifndef CPP_PERF_HPP
#define CPP_PERF_HPP

#include <chrono>
#include <algorithm>
#include <functional>
#include <ostream>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <thread>

namespace perf
{
    //===============================================
    //            Declaration of types
    //===============================================

    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point time_point;
    typedef clock::duration duration;
    typedef std::chrono::hours hours;
    typedef std::chrono::minutes minutes;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::nanoseconds nanoseconds;
    typedef std::function<bool(void)> perf_func;
    struct perf_case {
        perf_func f;
        std::string name;
        bool success;
        duration time;
    };
    class timer;
    class suite;
    class inline_timer;




    //===============================================
    //             Helper functions
    //===============================================
    
    /**
     * @brief Format a duration to be human readable.
     *
     * This function tries the different durations that are available through the
     * standard library. If the duration is longer than one hour the returned
     * string will represent the time rounded to hours. For shorter periods the
     * function will continue with minutes in the same manner.
     *
     * @parameter time The time duration to format
     * @return The duration formatted as a string
     */
    std::string format_duration(const duration& time)
    {
        std::stringstream strm;

        auto h = std::chrono::duration_cast<hours>(time);
        if(h.count() >= 1.) {
            strm << h.count() << " h";
            return strm.str();
        }

        auto m = std::chrono::duration_cast<minutes>(time);
        if(m.count() >= 1.) {
            strm << m.count() << " min";
            return strm.str();
        }

        auto s = std::chrono::duration_cast<seconds>(time);
        if(s.count() >= 1.) {
            strm << s.count() << " s";
            return strm.str();
        }

        auto ms = std::chrono::duration_cast<milliseconds>(time);
        if(ms.count() >= 1.) {
            strm << ms.count() << " ms";
            return strm.str();
        }

        auto mus = std::chrono::duration_cast<microseconds>(time);
        if(mus.count() >= 1.) {
            strm << mus.count() << " \u03BCs";
            return strm.str();
        }
    
        auto ns = std::chrono::duration_cast<nanoseconds>(time);
        strm << ns.count() << " ns";
        return strm.str();
    }

    /**
     * @brief Format code positions.
     *
     * The function parameter is optional as not every code position must be in a function.
     * The string will have the following format:
     * file:[function:]line
     *
     * @parameter file Path to the source file. You can use the __FILE__ macro
     * @parameter line Line of code in the file. You can use the __LINE__ macro
     * @parameter function The function containing this position. You can use the __FUNCTION__ macro
     * @return Code position formatted as a string.
     */
    std::string format_code_position(const std::string file, std::size_t line, std::string function = "")
    {
        std::stringstream strm;
        strm << file << ":";
        if(!function.empty()) strm << function << ":";
        strm << line;
        return strm.str();
    }

    /**
     * @brief Append charaters to lengthen a string.
     *
     * The filler character will be appended to the string str until it reaches the length len
     *
     * @param str The string which should be lengthened.
     * @param len New length to achieve.
     * @param filler The character to be used to lengthen the string
     */
    void lengthen_string(std::string& str, std::size_t len, char filler = ' ')
    {
        while(str.length() < len) str += filler;
    }
   



    //===============================================
    //             Class definitions
    //===============================================
   
    /**
     * @brief Class to measure times.
     *
     * This class is used to manually time between the call of the start and stop member functions.
     */
    class timer
    {
        private:
            clock m_clock;
            time_point m_start, m_stop;

        public:
            /**
             * @brief Construct a timer object.
             * @parameter start Decide if the timer should be created and started or only created.
             */
            timer(bool start = true)
            {
                if(start) m_start = m_clock.now();
            }

            /**
             * @brief This function starts the timer.
             */
            void start()
            {
                m_start = m_clock.now();
            }

            /**
             * @brief This function stops the timer.
             */
            void stop()
            {
                m_stop = m_clock.now();
            }

            /**
             * @brief This function returns the time interval between the start and the stop of the timer as a duration.
             * @return The elapsed time as a duration.
             */
            duration get_duration() const
            {
                return m_stop-m_start;
            }
    };

    /**
     * @brief Class used for automatic measurement using macros.
     *
     * This class is pobably of no use for you. It is used by the PERF_START()
     * and PERF_STOP() macros when using automatic measurement.
     */
    class inline_timer : public timer
    {
        friend std::ostream& operator<<(std::ostream& o, const inline_timer& timer);

        private:
            std::string m_file;
            std::size_t m_first_line, m_last_line;
            std::string m_function;

        public:
            /**
             * @brief Constructor used to create inline timers
             *
             * @parameter file The file where the inline timer is used in.
             * @parameter line The first line of measurement. This is basically where the timer is created.
             * @parameter function Optional parameter to indicate the function containing the measurement
             */
            inline_timer(const std::string& file, const std::size_t line, const std::string& function = "") :
                m_file(file), m_first_line(line), m_last_line(line), m_function(function) {}

            /**
             * @brief Set the line where the measurement ends
             */
            void set_last_line(std::size_t line)
            {
                if(line > m_first_line) m_last_line = line;
            }
    };

    /**
     * @brief Class to collect multiple test cases.
     *
     * Collect multiple test cases which are basically functions, functors or
     * lambdas with the signature bool(void). They can be ran all in order by
     * just one command. Exceptions will be caught and the observables success
     * and runtime are logged.
     *
     * An ostream operator helps you printing results out in a readable way.
     */
    class suite {
        friend std::ostream& operator<<(std::ostream& o, const suite& suite);

        private:
            std::vector<perf_case> m_cases;
            duration m_total_time;
            std::string m_name;

        public:

            /**
             * @brief Constructor to create an empty suite.
             *
             * This constructor creates a performance suite with a name.
             *
             * @parameter name The name of the suite. The default is PerfSuite
             */
            suite(const std::string& name = "PerfSuite") :
                m_name(name) {}

            /**
             * @brief Constructor to create a suite with cases.
             *
             * This constructor creates a suite and adds all cases given as an argument.
             *
             * @parameter cases The initial cases of the suite
             * @parameter name The name of the suite. The default is PerfSuite
             */
            suite(const std::vector<std::pair<std::string, perf_func> >& cases, const std::string& name = "PerfSuite") :
                m_name(name)
            {
                for(const auto& c : cases) add_case(c.first, c.second);
            }

            /**
             * @brief Add a case to the suite
             *
             * @parameter name The name of the case.
             * @parameter f The function, functor or lambda representing the case.
             */
            void add_case(const std::string& name, perf_func f)
            {
                m_cases.push_back({ f, name, false, duration() });
            }

            /**
             * @brief Set the name of a suite.
             *
             * @parameter name The new name of the suite
             */
            void set_name(const std::string& name)
            {
                m_name = name;
            }

            /**
             * @brief Get the name of a suite.
             *
             * @return The name of the suite
             */
            std::string get_name() const
            {
                return m_name;
            }

            /**
             * @brief Execute all cases of the suite
             */
            void run()
            {
                clock clk;
                time_point start, stop;
                m_total_time = nanoseconds(0);
                
                for(auto& c : m_cases) {
                    start = clk.now();
                    try {
                        c.success = c.f();
                    } catch(...) {
                        c.success = false;
                    }
                    stop = clk.now();
                    c.time = stop-start;
                    m_total_time += c.time;
                }
            }
    };




    //===============================================
    //              ostream operators
    //===============================================
   
    std::ostream& operator<<(std::ostream& o, const timer& t)
    {
        o << format_duration(t.get_duration());
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const inline_timer& timer)
    {
        o << format_code_position(timer.m_file, timer.m_first_line, timer.m_function);
        if(timer.m_last_line > timer.m_first_line) o << "-" << timer.m_last_line;
        o << ": " << format_duration(timer.get_duration());
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const suite& suite)
    {
        std::string vline = "";
        lengthen_string(vline, suite.m_name.length()+7, '=');

        if(suite.m_cases.empty()) {
            o << vline << std::endl;
            o << "Name:  " << suite.m_name << std::endl;
            o << "Empty Suite!" << std::endl;
            o << vline << std::endl;
        }
        
        std::string d, n;

        auto longest_name = std::max_element(std::begin(suite.m_cases), std::end(suite.m_cases), [](const perf_case& a, const perf_case& b) {
            return a.name.length() < b.name.length();
        })->name.length();
        auto longest_time = format_duration(std::max_element(std::begin(suite.m_cases), std::end(suite.m_cases), [](const perf_case& a, const perf_case& b) {
            return format_duration(a.time).length() < format_duration(b.time).length();
        })->time).length();

        std::string header_case = "Case", header_success = "Success", header_duration = "Duration", space1 = "", space2 = "", vsubline = "";
        lengthen_string(header_case, longest_name);
        lengthen_string(header_duration, longest_time);
        lengthen_string(space1, 4);
        lengthen_string(space2, 4);
        lengthen_string(vline, (header_case+space1+header_success+space2+header_duration).length(), '=');
        lengthen_string(vsubline, (header_case+space1+header_success+space2+header_duration).length(), '-');

        std::string name, success, duration;
        auto successful = 0ul;

        o << vline << std::endl;
        o << "Name:  " << suite.m_name << std::endl;
        o << header_case << space1 << header_success << space2 << header_duration << std::endl;
        o << vsubline << std::endl;
        for(auto c : suite.m_cases) {
            std::string n = c.name, d = format_duration(c.time), s = c.success ? "   1   " : "   0   ";
            lengthen_string(n, longest_name);
            lengthen_string(d, longest_time);
            o << n << space1 << s << space2 << d << std::endl;
            if(c.success) successful++;
        }
        o << vsubline << std::endl;

        name = "Total:";
        lengthen_string(name, longest_name);

        auto rate = double(successful)/double(suite.m_cases.size());
        rate *= 100.;
        rate = std::round(rate) / 100.;

        o << name << space1 << " " << rate << "  " << space2 << format_duration(suite.m_total_time) << std::endl;
        o << vline << std::endl;
        return o;
    }




    //===============================================
    //        Wrappers for global variables
    //===============================================
    std::ostream* inline_out_ptr()
    {
        static std::ostream* inline_out_ptr = &std::cout;
        return inline_out_ptr;
    }

    std::ostream* auto_out_ptr()
    {
        static std::ostream* auto_out_ptr = &std::cout;
        return auto_out_ptr;
    }

    std::stack<inline_timer>& inline_timer_stack()
    {
        static std::stack<inline_timer> inline_timer_stack;
        return inline_timer_stack;
    }
}




//===============================================
//                    Macros
//===============================================
#define PERF_START() \
     perf::inline_timer_stack().push(perf::inline_timer(__FILE__, __LINE__, __FUNCTION__)); \
     perf::inline_timer_stack().top().start();

#define PERF_STOP() \
     perf::inline_timer_stack().top().stop(); \
     perf::inline_timer_stack().top().set_last_line(__LINE__); \
     *perf::inline_out_ptr() << perf::inline_timer_stack().top() << std::endl; \
     perf::inline_timer_stack().pop();

#define PERF_BEGIN(name) \
    int main() \
    { \
        perf::suite perf_auto_suite; \
        perf_auto_suite.set_name(name);

#define PERF_END() \
        perf_auto_suite.run(); \
        *perf::auto_out_ptr() << perf_auto_suite << std::endl; \
    }

#define PERF_CASE(name, ...) \
    perf_auto_suite.add_case(name, []() { \
        __VA_ARGS__ \
        return true; \
    });

#endif
