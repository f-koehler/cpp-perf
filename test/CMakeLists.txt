find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# macro to add boost test executable
macro(cppperf_add_test_executable target sources)
    cppperf_add_executable(${target} ${sources})
    target_compile_definitions(${target} PRIVATE BOOST_TEST_DYN_LINK)
    target_link_libraries(${target} PRIVATE Boost::unit_test_framework)
endmacro()

cppperf_add_test_executable(format_code_position test_format_code_position.cpp)
cppperf_add_test_executable(format_duration test_format_duration.cpp)
cppperf_add_test_executable(timer test_timer.cpp)

add_test("test.format_code_position" format_code_position)
add_test("test.format_duration" format_duration)
add_test("test.timer" timer)
