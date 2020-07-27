cmake_minimum_required(VERSION 3.10)
project(benchmark)
set(CMAKE_CXX_STANDARD 14)

find_package(CGAL REQUIRED QUIET OPTIONAL_COMPONENTS Core )

add_executable(construction_benchmark construction_benchmark.cpp benchmark.h)

target_link_libraries(construction_benchmark CGAL::CGAL)
