cmake_minimum_required(VERSION 3.9.0)
project(vmath)

find_package(PkgConfig REQUIRED)

set (vmath_H src/vmath.h)
set (vmath_CPP src/vmath.cpp)

include_directories(src)

add_executable(vmath_tests ${vmath_CPP} ${tests_CPP})

set_property(TARGET vmath_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET vmath_tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(vmath_tests PUBLIC ${CppUnit_INCLUDE_DIRS})
target_link_libraries(vmath_tests ${CppUnit_LIBRARIES})
target_compile_options(vmath_tests PUBLIC ${CppUnit_CFLAGS_OTHER})
