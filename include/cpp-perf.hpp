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

    std::string format_duration(const duration& time)
    {
        std::stringstream strm;

        auto h = std::chrono::duration_cast<hours>(time);
        if(h.count() > 1.) {
            strm << h.count() << "h";
            return strm.str();
        }

        auto m = std::chrono::duration_cast<minutes>(time);
        if(m.count() > 1.) {
            strm << m.count() << "m";
            return strm.str();
        }

        auto s = std::chrono::duration_cast<seconds>(time);
        if(s.count() > 1.) {
            strm << s.count() << "s";
            return strm.str();
        }

        auto ms = std::chrono::duration_cast<milliseconds>(time);
        if(ms.count() > 1.) {
            strm << ms.count() << "ms";
            return strm.str();
        }

        auto mus = std::chrono::duration_cast<microseconds>(time);
        if(mus.count() > 1.) {
            strm << mus.count() << "\u03BCs";
            return strm.str();
        }
    
        auto ns = std::chrono::duration_cast<nanoseconds>(time);
        strm << ns.count() << "ns";
        return strm.str();
    }

    std::string format_bool(bool b)
    {
        return b ? "true " : "false";
    }
    std::string format_code_position(const std::string file, std::size_t line, std::string function = "")
    {
        std::stringstream strm;
        strm << file << ":";
        if(!function.empty()) strm << function << ":";
        strm << line;
        return strm.str();
    }
    void fill_string(std::string& str, std::size_t len, char filler = ' ')
    {
        while(str.length() < len) str += filler;
    }
   

    class timer
    {
        private:
            clock m_clock;
            time_point m_start, m_stop;

        public:
            timer(bool start = true) { if(start) m_start = m_clock.now(); }

            void start() {
                m_start = m_clock.now();
            }

            void stop() {
                m_stop = m_clock.now();
            }

            duration get_duration() const { return m_stop-m_start; }
    };

    class inline_timer : public timer
    {
        friend std::ostream& operator<<(std::ostream& o, const inline_timer& timer);

        private:
            std::string m_file;
            std::size_t m_first_line, m_last_line;
            std::string m_function;

        public:
            inline_timer(const std::string& file, const std::size_t line, const std::string& function = "") : m_file(file), m_first_line(line), m_last_line(line), m_function(function) {}

            void set_last_line(std::size_t line) { if(line > m_first_line) m_last_line = line; }
    };

    class suite {
        friend std::ostream& operator<<(std::ostream& o, const suite& suite);

        private:
            std::vector<perf_case> m_cases;
            duration m_total_time;
            std::string m_name;

        public:
            suite(const std::string& name = "PerfSuite") : m_name(name) {}
            suite(const std::vector<std::pair<std::string, perf_func> >& cases, const std::string& name = "PerfSuite") : m_name(name) { for(const auto& c : cases) add_case(c.first, c.second); }
            suite(const suite& suite) : m_cases(suite.m_cases), m_name(suite.m_name) {}

            void add_case(const std::string& name, perf_func f) { m_cases.push_back({ f, name, false, duration() }); }

            void set_name(const std::string& name) { m_name = name; }

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
        fill_string(vline, suite.m_name.length()+7, '=');

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
        fill_string(header_case, longest_name);
        fill_string(header_duration, longest_time);
        fill_string(space1, 4);
        fill_string(space2, 4);
        fill_string(vline, (header_case+space1+header_success+space2+header_duration).length(), '=');
        fill_string(vsubline, (header_case+space1+header_success+space2+header_duration).length(), '-');

        std::string name, success, duration;
        auto successful = 0ul;

        o << vline << std::endl;
        o << "Name:  " << suite.m_name << std::endl;
        o << header_case << space1 << header_success << space2 << header_duration << std::endl;
        o << vsubline << std::endl;
        for(auto c : suite.m_cases) {
            std::string n = c.name, d = format_duration(c.time), s = c.success ? "   1   " : "   0   ";
            fill_string(n, longest_name);
            fill_string(d, longest_time);
            o << n << space1 << s << space2 << d << std::endl;
            if(c.success) successful++;
        }
        o << vsubline << std::endl;

        name = "Total:";
        fill_string(name, longest_name);

        auto rate = double(successful)/double(suite.m_cases.size());
        rate *= 100.;
        rate = std::round(rate) / 100.;

        o << name << space1 << " " << rate << "  " << space2 << format_duration(suite.m_total_time) << std::endl;
        o << vline << std::endl;
        return o;
    }

    std::ostream* inline_out_ptr() {
        static std::ostream* inline_out_ptr = &std::cout;
        return inline_out_ptr;
    }

    std::stack<inline_timer>& inline_timer_stack() {
        static std::stack<inline_timer> inline_timer_stack;
        return inline_timer_stack;
    }
}

#define PERF_START() \
     perf::inline_timer_stack().push(perf::inline_timer(__FILE__, __LINE__, __FUNCTION__)); \
     perf::inline_timer_stack().top().start();

#define PERF_STOP() \
     perf::inline_timer_stack().top().stop(); \
     perf::inline_timer_stack().top().set_last_line(__LINE__); \
     *perf::inline_out_ptr() << perf::inline_timer_stack().top() << std::endl; \
     perf::inline_timer_stack().pop();

#define PERF_BEGIN(name) \
    int main() { \
        perf::suite perf_auto_suite; \
        perf_auto_suite.set_name(name);

#define PERF_END() \
        perf_auto_suite.run(); \
        std::cout << perf_auto_suite << std::endl; \
    }

#define PERF_CASE(name, ...) \
    perf_auto_suite.add_case(name, []() { \
        __VA_ARGS__ \
        return true; \
    });

/* #define PERF_CASE(name ) \ */
/*     []() */ 

/* #ifdef PERF_AUTO_SUITE */
/*     int main() { */
/*          perf::auto_suite().set_name(PERF_AUTO_SUITE); */
/*          perf::auto_suite().run(); */
/*          std::cout << perf::auto_suite() << std::endl; */
/*          return 0; */
/*      } */
/* #endif */

#endif
