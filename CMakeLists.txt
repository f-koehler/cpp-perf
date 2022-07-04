cmake_minimum_required(VERSION 3.10)
project(cppperf LANGUAGES CXX)

set(CMAKE_CXX_STAND 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# check if IPO is supported
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)

if(NOT result)
    message(WARNING "IPO is not supported: ${output}")
endif()

# options
option(CPPPERF_TESTS "Enable compilation of unit tests" ON)
option(CPPPERF_EXAMPLES "Enable compilation of examples" ON)

# macro to enable warnings
macro(cppperf_enable_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wconversion -Wsign-conversion -pedantic -Werror -pedantic-errors>
    )
endmacro()

# determin threading library
find_package(Threads)

# macro to add executable
macro(cppperf_add_executable target sources)
    add_executable(${target} ${sources})
    cppperf_enable_warnings(${target})
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-O>
    )
    target_link_libraries(${target} PRIVATE Threads::Threads)
endmacro()

include_directories(include)

if(${CPPPERF_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()

if(${CPPPERF_EXAMPLES})
    add_subdirectory(example)
endif()
