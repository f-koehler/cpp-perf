#define BOOST_TEST_MODULE TestFormatCodePosition
#include <boost/test/unit_test.hpp>

#include <cpp-perf.hpp>

BOOST_AUTO_TEST_CASE(format_code_position)
{
    BOOST_CHECK_EQUAL(perf::format_code_position("main.cpp", 5), "main.cpp:5");
    BOOST_CHECK_EQUAL(perf::format_code_position("main.cpp", 5, "func"), "main.cpp:func:5");
}
