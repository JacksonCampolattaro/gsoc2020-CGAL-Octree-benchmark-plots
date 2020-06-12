cmake_minimum_required(VERSION 3.10)
project(benchmark)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp benchmark.h)

find_package(CGAL REQUIRED QUIET OPTIONAL_COMPONENTS Core )

add_executable(benchmark ${SOURCE_FILES})

target_link_libraries(benchmark CGAL::CGAL)
