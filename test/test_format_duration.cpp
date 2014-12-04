#define BOOST_TEST_MODULE TestFormatDuration
#include <boost/test/unit_test.hpp>

#include <cpp-perf.hpp>

BOOST_AUTO_TEST_CASE(hours)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::hours(1)), "1 h");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::minutes(60)), "1 h");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::minutes(120)), "2 h");
}

BOOST_AUTO_TEST_CASE(minutes)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::minutes(1)), "1 min");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::seconds(60)), "1 min");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::seconds(120)), "2 min");
}

BOOST_AUTO_TEST_CASE(seconds)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::seconds(1)), "1 s");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::milliseconds(1000)), "1 s");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::milliseconds(2000)), "2 s");
}

BOOST_AUTO_TEST_CASE(milliseconds)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::milliseconds(1)), "1 ms");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::microseconds(1000)), "1 ms");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::microseconds(2000)), "2 ms");
}

BOOST_AUTO_TEST_CASE(microseconds)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::microseconds(1)), "1 \u03BCs");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::nanoseconds(1000)), "1 \u03BCs");
    BOOST_CHECK_EQUAL(perf::format_duration(perf::nanoseconds(2000)), "2 \u03BCs");
}

BOOST_AUTO_TEST_CASE(nanoseconds)
{
    BOOST_CHECK_EQUAL(perf::format_duration(perf::nanoseconds(1)), "1 ns");
}
