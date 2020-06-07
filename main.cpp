//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include "gnuplot_i.hpp"
#include <iostream>
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

static const int repetitions = 10;

int bench_old(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  int duration = 0;
  for (int i = 0; i < repetitions; ++i) {

    auto start = high_resolution_clock::now();

    auto oldOctree = OldOctree((Traits()), input_iterator_first,
                               input_iterator_beyond, point_map, normal_map);
    oldOctree.createTree();

    auto end = high_resolution_clock::now();
    duration += duration_cast<microseconds>(end - start).count();
  }

  return duration / repetitions;
}

int bench_new(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  int duration = 0;
  for (int i = 0; i < repetitions; ++i) {

    auto start = high_resolution_clock::now();

    NewOctree newOctree(points, point_map, normal_map);

    auto end = high_resolution_clock::now();
    duration += duration_cast<microseconds>(end - start).count();
  }

  return duration / repetitions;
}

int main() {

  std::vector<int> x, yOld, yNew;

  for (int N = 1; N < 1000; N += 1 + (N / 100)) {

    cout << N << endl;

    // Generate random point set
    Point_set points;
    CGAL::Random_points_on_sphere_3<Point> generator;
    points.reserve(N);
    for (std::size_t i = 0; i < N; ++i)
      points.insert(*(generator++));

    x.push_back(N);
    yOld.push_back(bench_old(points));
    yNew.push_back(bench_new(points));
  }


  Gnuplot plot("lines");
  plot.set_title("Comparison of Old and New Algorithms for Constructing an Octree");
  plot.set_ylabel("Time to Build a Tree (Microseconds)");
  plot.set_xlabel("Number of Points Added");
  plot.plot_xy(x, yOld, "Old");
  plot.plot_xy(x, yNew, "New");


  cout << endl << "Press ENTER to continue..." << endl;
  cin.clear();
  cin.ignore(cin.rdbuf()->in_avail());
  cin.get();
}
