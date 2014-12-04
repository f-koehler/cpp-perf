find_package(Boost REQUIRED COMPONENTS unit_test_framework REQUIRED)

add_definitions(-DBOOST_TEST_DYN_LINK)

function(add_perf_test TEST_NAME TEST_SOURCE)
    get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    target_link_libraries(${TEST_TARGET} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

add_perf_test("core.format_duration" test_format_duration.cpp)
add_perf_test("core.timer"           test_timer.cpp)
