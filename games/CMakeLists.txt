cmake_minimum_required(VERSION 3.0)
project(games LANGUAGES C CXX)

include_directories(
	${PROJECT_SOURCE_DIR}
		${PROJECT_SOURCE_DIR}/tarkov
		../vmath/src
)


set(COMMON_SOURCES
  Includes.cpp
  tarkov/tarkovread.cpp
)

add_library(games STATIC ${COMMON_SOURCES})
target_link_libraries(games lg_common)
