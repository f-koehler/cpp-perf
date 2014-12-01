#ifndef UTIL_HPP
#define UTIL_HPP

#include <cpp-perf/types.hpp>
#include <sstream>

namespace perf
{
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
}

#endif
