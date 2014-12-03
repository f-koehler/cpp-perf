add_executable(auto auto.cpp)
add_executable(inline inline.cpp)
add_executable(manual manual.cpp)

if(${PERF_TESTS})
    add_test("example.auto"   auto)
    add_test("example.inline" inline)
    add_test("example.manual" manual)
endif()
