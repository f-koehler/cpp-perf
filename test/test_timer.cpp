#define BOOST_TEST_MODULE TestTimer
#include <boost/test/unit_test.hpp>

#include <cpp-perf.hpp>

BOOST_AUTO_TEST_CASE(constructor_started)
{
    perf::timer t;
    std::this_thread::sleep_for(perf::milliseconds(300));
    t.stop();
    std::ostringstream strm;
    strm << t;

    BOOST_CHECK_EQUAL(strm.str(), "300 ms");
}

BOOST_AUTO_TEST_CASE(constructor_paused)
{
    perf::timer t(false);
    t.start();
    std::this_thread::sleep_for(perf::seconds(1));
    t.stop();
    std::ostringstream strm;
    strm << t;

    BOOST_CHECK_EQUAL(strm.str(), "1000 ms");
}

BOOST_AUTO_TEST_CASE(start_stop)
{
    perf::timer t(false);
    t.start();
    std::this_thread::sleep_for(perf::milliseconds(80));
    t.stop();
    std::ostringstream strm;
    strm << t;

    BOOST_CHECK_EQUAL(strm.str(), "80 ms");
}

BOOST_AUTO_TEST_CASE(get_duration)
{
    perf::timer t(false);
    t.start();
    std::this_thread::sleep_for(perf::milliseconds(42));
    t.stop();
    std::ostringstream strm;
    strm << t;

    BOOST_CHECK_EQUAL(perf::format_duration(t.duration()), "42 ms");
}
