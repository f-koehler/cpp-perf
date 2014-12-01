#ifndef CPP_PERF_HPP
#define CPP_PERF_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <ostream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace perf {
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

    std::string format_bool(bool b) { return b ? "true " : "false"; }

    void make_string_length(std::string& str, size_t len, char filler = ' ') { while(str.length() < len) str += filler; }
   

    class timer
    {
        friend std::ostream& operator<<(std::ostream& o, const timer& timer);

        private:
            clock m_clock;
            time_point m_start, m_stop;
            bool m_named;
            std::string m_name;

        public:
            timer() : m_named(false) {}
            timer(const std::string& name) : m_named(true), m_name(name) {}

            static std::string format_name(const std::string& file, unsigned line, const std::string& function) {
                std::stringstream strm;
                strm << file << ":" << line << ":" << function << ":";
                return strm.str();
            }

            void start() {
                m_start = m_clock.now();
            }

            timer& stop() {
                m_stop = m_clock.now();
                return *this;
            }

            duration get_duration() { return m_stop-m_start; }
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

    std::ostream& operator<<(std::ostream& o, const timer& timer)
    {
        if(timer.m_named) o << timer.m_name << "\t";
        o << format_duration(timer.m_stop-timer.m_start);
        return o;
    }

     std::ostream& operator<<(std::ostream& o, const suite& suite)
    {
        std::string d, n;

        auto longest_name = std::max_element(std::begin(suite.m_cases), std::end(suite.m_cases), [](const perf_case& a, const perf_case& b) {
            return a.name.length() < b.name.length();
        })->name.length();
        auto longest_time = format_duration(std::max_element(std::begin(suite.m_cases), std::end(suite.m_cases), [](const perf_case& a, const perf_case& b) {
            return format_duration(a.time).length() < format_duration(b.time).length();
        })->time).length();

        std::string header_case = "Case", header_success = "Success", header_duration = "Duration", space1 = "", space2 = "", vline = "", vsubline = "";
        make_string_length(header_case, longest_name);
        make_string_length(header_duration, longest_time);
        make_string_length(space1, 4);
        make_string_length(space2, 4);
        make_string_length(vline, (header_case+space1+header_success+space2+header_duration).length(), '=');
        make_string_length(vsubline, (header_case+space1+header_success+space2+header_duration).length(), '-');

        std::string name, success, duration;
        auto successful = 0ul;

        o << vline << std::endl;
        o << "Name:  " << suite.m_name << std::endl;
        o << header_case << space1 << header_success << space2 << header_duration << std::endl;
        o << vsubline << std::endl;
        for(auto c : suite.m_cases) {
            std::string n = c.name, d = format_duration(c.time), s = c.success ? "   1   " : "   0   ";
            make_string_length(n, longest_name);
            make_string_length(d, longest_time);
            o << n << space1 << s << space2 << d << std::endl;
            if(c.success) successful++;
        }
        o << vsubline << std::endl;

        name = "Total:";
        make_string_length(name, longest_name);

        auto rate = double(successful)/double(suite.m_cases.size());
        rate *= 100.;
        rate = std::round(rate) / 100.;

        o << name << space1 << " " << rate << "  " << space2 << format_duration(suite.m_total_time) << std::endl;
        o << vline << std::endl;
        return o;
    }
}

#endif
