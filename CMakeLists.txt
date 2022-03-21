cmake_minimum_required(VERSION 3.19)
project(simulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(simulator simulator.cpp simulator.h phase1.cpp phase1.h phase2.cpp phase2.h LableTable.h )
