//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include "plotting.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>
#include <CGAL/point_generators_3.h>

#include <CGAL/Octree.h>
#include <CGAL/Octree/IO.h>

#include "Octree.h"
#include <CGAL/Shape_detection/Efficient_RANSAC/Efficient_RANSAC_traits.h>
#include "Octree_3.h"

#include <chrono>
#include <iostream>
#include <istream>
#include <CGAL/random_simplify_point_set.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
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

// Defining the Improved Octree
typedef CGAL::Octree::Octree<Point_set, typename Point_set::Point_map> ImprovedOctree;
typedef ImprovedOctree::FT FT;

// Defining the kD tree
typedef CGAL::Search_traits_3<Kernel> Kd_tree_traits;
typedef CGAL::Orthogonal_k_neighbor_search<Kd_tree_traits> Kd_tree_search;
typedef Kd_tree_search::Tree Kd_tree;

typedef std::chrono::microseconds time_unit;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

using std::cout;
using std::endl;

#define SAMPLES_PER_TEST 1000

#define BUCKET_SIZE 20
#define MAX_DEPTH 10

int bench_old(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  auto start = high_resolution_clock::now();

  auto oldOctree = OldOctree((Traits()), input_iterator_first,
                             input_iterator_beyond, point_map, normal_map,
                             0, BUCKET_SIZE, MAX_DEPTH);

  oldOctree.createTree();

  auto end = high_resolution_clock::now();
  return duration_cast<time_unit>(end - start).count();
}

int bench_new(Point_set points) {

  auto point_map = points.point_map();
  auto normal_map = points.normal_map();
  auto input_iterator_first = points.begin();
  auto input_iterator_beyond = points.end();

  auto start = high_resolution_clock::now();

  NewOctree newOctree(points, point_map, normal_map);
  newOctree.refine(MAX_DEPTH, BUCKET_SIZE);

  auto end = high_resolution_clock::now();
  return duration_cast<time_unit>(end - start).count();
}

int bench_improved(Point_set points) {

  auto point_map = points.point_map();

  auto start = high_resolution_clock::now();

  ImprovedOctree improvedOctree(points, point_map);
  improvedOctree.refine(MAX_DEPTH, BUCKET_SIZE);

  auto end = high_resolution_clock::now();
  return duration_cast<time_unit>(end - start).count();
}

int bench_kdtree(Point_set points) {

  auto start = high_resolution_clock::now();

  Kd_tree kd_tree(points.points().begin(), points.points().end());
  kd_tree.build();

  auto end = high_resolution_clock::now();
  return duration_cast<time_unit>(end - start).count();
}

void synthetic_bench(int max_points, std::string file_path) {

  std::vector<int> x, y_old, y_new, y_improved, y_kd;

  for (int N = 1; N < max_points; N += 1 + (N / 20)) {

    cout << N << endl;
    x.push_back(N);

    int oldResult = 0;
    int newResult = 0;
    int improvedResult = 0;
    int kdResult = 0;

    for (int sample = 0; sample < SAMPLES_PER_TEST; ++sample) {

      // Generate random point set
      Point_set points;
      CGAL::Random_points_on_sphere_3<Point> generator;
      points.reserve(N);
      for (std::size_t i = 0; i < N; ++i)
        points.insert(*(generator++));

      oldResult += bench_old(points);
      newResult += bench_new(points);
      improvedResult += bench_improved(points);
      kdResult += bench_kdtree(points);

    }

    y_old.push_back(oldResult / SAMPLES_PER_TEST);
    y_new.push_back(newResult / SAMPLES_PER_TEST);
    y_improved.push_back(improvedResult / SAMPLES_PER_TEST);
    y_kd.push_back(kdResult / SAMPLES_PER_TEST);

  }

  line_plot(x, "Number of Points Added",
            {
                    {y_old, "Old"},
                    {y_new, "New"},
                    {y_improved, "Improved"},
                    {y_kd, "kD Tree"}
            }, "Time to Build a Tree (Microseconds)",
            "Comparison of Algorithms for Constructing an Octree",
            file_path
  );
}

void photogrammetry_bench(int max_points, std::string file_path) {

  std::vector<int> x, y_old, y_new, y_improved, y_kd;

  // Read example data from file
  std::ifstream stream("../data/archer_original.ply");
  Point_set points;
  stream >> points;

  double percent_to_remove = 100 - (100.0 * max_points / points.number_of_points());
  points.remove(CGAL::random_simplify_point_set(points, percent_to_remove), points.end());

  while (points.number_of_points() > 10) {

    cout << points.size() << endl;

    points.remove(CGAL::random_simplify_point_set(points, 0.1) - 1, points.end());

    x.insert(x.begin(), points.size());

    y_old.insert(y_old.begin(), bench_old(points));
    y_new.insert(y_new.begin(), bench_new(points));
    y_improved.insert(y_improved.begin(), bench_improved(points));
    y_kd.insert(y_kd.begin(), bench_kdtree(points));
  }

  line_plot(x, "Number of Points Added",
            {
                    {y_old,      "Old"},
                    {y_new,      "New"},
                    {y_improved, "Improved"},
                    {y_kd, "kD Tree"}
            }, "Time to Build a Tree (Microseconds)",
            "Comparison of Algorithms for Constructing an Octree",
            file_path
  );
}

int main() {

  synthetic_bench(
          2000,
          "../results/octree-benchmark-plot-spherical_shell-release_mode-2000_points-4_way.png");
//  synthetic_bench(
//          10000,
//          "../results/octree-benchmark-plot-spherical_shell-release_mode-10000_points-4_way.png");
//  synthetic_bench(
//          1000000,
//          "../results/octree-benchmark-plot-spherical_shell-release_mode-1000000_points-4_way.png");

  photogrammetry_bench(
          2000,
          "../results/octree-benchmark-plot-statue_scan-release_mode-2000_points-4_way.png");
//  photogrammetry_bench(
//          10000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-10000_points-4_way.png");
//  photogrammetry_bench(
//          1000000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-1000000_points-4_way.png");
}
