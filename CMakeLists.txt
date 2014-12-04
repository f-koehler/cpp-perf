cmake_minimum_required(VERSION 2.8)
project(primefac)

# options
option(PERF_TESTS    "Enable compilation of unit tests"  OFF)
option(PERF_EXAMPLES "Enable compilation of examples"    ON)
option(PERF_OLD_COMP "Enable this when using GCC < 4.8" OFF)

# enable warnings
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-Wall -Wextra -pedantic)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(/W4 /WX /EHsc)
endif()

# enable c++11 features
if(${PERF_OLD_COMP})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++0x)
    endif()
else()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
    endif()
endif()

include_directories(include)

if(${PERF_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()

if(${PERF_EXAMPLES})
    add_subdirectory(example)
endif()
