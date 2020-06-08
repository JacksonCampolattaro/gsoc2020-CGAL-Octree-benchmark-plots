//
// Created by jackcamp on 6/8/20.
//

#ifndef BENCHMARK_BENCHMARK_H
#define BENCHMARK_BENCHMARK_H

#include <chrono>

#include "Octree.h"
#include <CGAL/Shape_detection/Efficient_RANSAC/Efficient_RANSAC_traits.h>
#include "Octree_3.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/point_generators_3.h>



using std::cin;
using std::cout;
using std::endl;

using namespace std::chrono;

// Defining the kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_set_3<Point> Point_set;
typedef CGAL::Point_set_3<Point> Point_set;

// Defining the Old Octree
typedef CGAL::Shape_detection::Efficient_RANSAC_traits
        <Kernel, Point_set, typename Point_set::Point_map, typename Point_set::Vector_map>
        Traits;
typedef CGAL::Shape_detection::internal::Octree
        <CGAL::Shape_detection::internal::DirectPointAccessor<Traits>>
        OldOctree;

// Defining the New Octree
typedef CGAL::OCTREE::Octree
        <Kernel, Point_set, typename Point_set::Point_map, typename Point_set::Vector_map>
        NewOctree;


int bench_old(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  auto start = high_resolution_clock::now();

  auto oldOctree = OldOctree((Traits()), input_iterator_first,
                             input_iterator_beyond, point_map, normal_map);
  oldOctree.createTree();

  auto end = high_resolution_clock::now();
  return duration_cast<microseconds>(end - start).count();
}

int bench_new(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  auto start = high_resolution_clock::now();

  NewOctree newOctree(points, point_map, normal_map);

  auto end = high_resolution_clock::now();
  return duration_cast<microseconds>(end - start).count();
}

#endif //BENCHMARK_BENCHMARK_H
