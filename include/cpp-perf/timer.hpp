#ifndef TIMER_HPP
#define TIMER_HPP

#include <cpp-perf/types.hpp>
#include <cpp-perf/util.hpp>
#include <ostream>
#include <string>
#include <sstream>
#include <iostream>

#ifdef PERF_ENABLE_INLINE
#define PERF_TIME(x) { \
    perf::perf_timer timer(perf::perf_timer::format_name(__FILE__, __LINE__, __FUNCTION__)); \
    timer.start(); \
    x; \
    timer.stop();  \
    std::cout << timer << std::endl; \
}
#else
#define PERF_TIME(x) x
#endif

namespace perf
{
    class perf_timer
    {
        friend std::ostream& operator<<(std::ostream& o, const perf_timer& timer);

        private:
            clock m_clock;
            time_point m_start, m_stop;
            bool m_named;
            std::string m_name;

        public:
            perf_timer() : m_named(false) {}
            perf_timer(const std::string& name) : m_named(true), m_name(name) {}

            static std::string format_name(const std::string& file, unsigned line, const std::string& function) {
                std::stringstream strm;
                strm << file << ":" << line << ":" << function << ":";
                return strm.str();
            }

            void start() {
                m_start = m_clock.now();
            }

            perf_timer& stop() {
                m_stop = m_clock.now();
                return *this;
            }

            duration get_duration() { return m_stop-m_start; }
    };

    std::ostream& operator<<(std::ostream& o, const perf_timer& timer)
    {
        if(timer.m_named) o << timer.m_name << "\t";
        o << format_duration(timer.m_stop-timer.m_start);
        return o;
    }
}

#endif
