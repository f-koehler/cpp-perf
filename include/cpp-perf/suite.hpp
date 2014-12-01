#ifndef SUITE_HPP 
#define SUITE_HPP 

#include <cpp-perf/types.hpp>
#include <cpp-perf/util.hpp>

#include <algorithm>
#include <vector>
#include <chrono>
#include <ostream>
#include <string>
#include <utility>
#include <cmath>

namespace perf 
{
    class perf_suite {
        friend std::ostream& operator<<(std::ostream& o, const perf_suite& suite);

        private:
            std::vector<perf_case> m_cases;
            duration m_total_time;
            std::string m_name;

        public:
            perf_suite(const std::string& name = "perf_suite") : m_name(name) {}
            perf_suite(const std::vector<std::pair<std::string, perf_func> >& cases, const std::string& name = "perf_suite") : m_name(name) { for(const auto& c : cases) add_case(c.first, c.second); }
            perf_suite(const perf_suite& suite) : m_cases(suite.m_cases), m_name(suite.m_name) {}

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

    std::ostream& operator<<(std::ostream& o, const perf_suite& suite)
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
