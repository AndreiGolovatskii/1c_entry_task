cmake_minimum_required(VERSION 3.17)

add_subdirectory(third-party/googletest EXCLUDE_FROM_ALL)

project(1c_entry)

set(CMAKE_CXX_STANDARD 20)

add_library(solver game_state.hh game_solver.hh game_solver.cpp game_state.cpp)

add_executable(1c_entry main.cpp)
target_link_libraries(1c_entry solver)


add_executable(Tests tests.cpp)
target_link_libraries(Tests solver)

find_package(Threads REQUIRED)
include(GoogleTest)
target_link_libraries(Tests gtest_main)
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} Threads::Threads)
gtest_discover_tests(Tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})