#define BOOST_TEST_MODULE TestFormatCodePosition
#include <boost/test/unit_test.hpp>

#include <cpp-perf.hpp>

BOOST_AUTO_TEST_CASE(format_code_position)
{
    std::ostringstream strm1;
    strm1 << '#' << std::this_thread::get_id() << ":main.cpp:5";
    BOOST_CHECK_EQUAL(perf::format_code_position("main.cpp", 5), strm1.str());

    std::ostringstream strm2;
    strm2 << '#' << std::this_thread::get_id() << ":main.cpp:func:5";
    BOOST_CHECK_EQUAL(perf::format_code_position("main.cpp", 5, "func"), strm2.str());
}
