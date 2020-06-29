cmake_minimum_required(VERSION 3.9.0)
project(vmath)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CppUnit REQUIRED cppunit)

set (vmath_H src/vmath.h)
set (vmath_CPP src/vmath.cpp)
set (tests_H
        tests/Aabb3Test.h
        tests/ConvTest.h
        tests/Matrix3fTest.h
        tests/Matrix4fTest.h
        tests/MinMaxTest.h
        tests/QuatTest.h
        tests/Vector2fTest.h
        tests/Vector3fTest.h
        tests/testCommon.h)
set (tests_CPP
        tests/Aabb3Test.cpp
        tests/ConvTest.cpp
        tests/Matrix3fTest.cpp
        tests/Matrix4fTest.cpp
        tests/MinMaxTest.cpp
        tests/QuatTest.cpp
        tests/Vector2fTest.cpp
        tests/Vector3fTest.cpp
        tests/testMain.cpp)

link_directories(${CppUnit_LIBRARY_DIRS})
include_directories(src)

add_executable(vmath_tests ${vmath_CPP} ${tests_CPP})

set_property(TARGET vmath_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET vmath_tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(vmath_tests PUBLIC ${CppUnit_INCLUDE_DIRS})
target_link_libraries(vmath_tests ${CppUnit_LIBRARIES})
target_compile_options(vmath_tests PUBLIC ${CppUnit_CFLAGS_OTHER})
