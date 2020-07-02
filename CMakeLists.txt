cmake_minimum_required(VERSION 3.16)
project(privateCheese LANGUAGES C CXX)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lX11")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_C_STANDARD 11)
SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

include_directories(
        vmread
        vmread/hlapi
        tarkov
        vmath/src
)

add_compile_options(
        "-DLMODE=MODE_EXTERNAL")

set(SOURCES
        main.cpp
        tarkovread.cpp
        vmread/mem.c
        vmread/wintools.c
        vmread/pmparser.c
        vmread/vmmem.c
        vmread/hlapi/windll.cpp
        vmread/hlapi/winprocess.cpp
        vmread/hlapi/winprocesslist.cpp
        vmath/src/vmath.cpp
        tarkov/Includes.cpp
        render.cpp render.h)


add_executable(privateCheese ${SOURCES}
        )
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory("common"          "${CMAKE_BINARY_DIR}/common"   )
link_directories(${X11_LIBRARIES})
target_link_libraries(privateCheese
        ${X11_LIBRARIES})
target_link_libraries(privateCheese
        GLU
        GL)
target_link_libraries(privateCheese
        lg_common
        )
