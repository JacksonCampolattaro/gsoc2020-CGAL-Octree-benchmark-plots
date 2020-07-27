//
// Created by jackcamp on 7/27/20.
//

#include "plotting.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/Octree.h>
#include <CGAL/Octree/IO.h>

#include <chrono>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_set_3<Point> Point_set;

// Define the Octree
typedef CGAL::Octree::Octree<Point_set, typename Point_set::Point_map> Octree;

// Define the kD_tree
typedef CGAL::Search_traits_3<Kernel> Kd_tree_traits;
typedef CGAL::Orthogonal_k_neighbor_search<Kd_tree_traits> Kd_tree_search;
typedef Kd_tree_search::Tree Kd_tree;

typedef std::chrono::nanoseconds time_unit;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

#define SAMPLES_PER_TEST 10

int bench_search_naive(Point_set points, Point search_point) {

  auto start = high_resolution_clock::now();


  FT distance_nearest = std::numeric_limits<FT>::max();

  Point naive_nearest;
  for (auto &p : points.points()) {

    FT distance_current = CGAL::squared_distance(p, search_point);
    if (distance_current < distance_nearest) {

      distance_nearest = distance_current;
      naive_nearest = p;
    }
  }

  auto end = high_resolution_clock::now();

  std::cout << "naive --> " << naive_nearest << std::endl;
  return duration_cast<time_unit>(end - start).count();
}

int bench_search_kd_tree(Kd_tree &kd_tree, Point search_point) {

  auto start = high_resolution_clock::now();

  Kd_tree_search search(kd_tree, search_point, 1);

  auto end = high_resolution_clock::now();

  for (auto p : search)
    std::cout << "kd_tree --> " << p.first << std::endl;
  return duration_cast<time_unit>(end - start).count();
}


void synthetic_bench(std::size_t num_points, std::string file_path) {

  std::vector<int> x, y_naive, y_kdtree;

  for (std::size_t N = 0; N < num_points; ++N) {

    x.push_back(N);
    std::cout << N << std::endl;

    // Generate a dataset
    Point_set points;
    CGAL::Random_points_on_sphere_3<Point> generator;
    points.reserve(N);
    for (std::size_t i = 0; i < N; ++i)
      points.insert(*(generator++));

    // Generate a search point
    Point search_point = *generator++;

    // Naive benchmark
    int naive_time = 0;
    for (int i = 0; i < SAMPLES_PER_TEST; ++i) {

      naive_time += bench_search_naive(points, search_point) / SAMPLES_PER_TEST;
    }
    y_naive.push_back(naive_time);

    // Kd tree benchmark
    Kd_tree kd_tree(points.points().begin(), points.points().end());
    kd_tree.build();
    y_kdtree.push_back(bench_search_kd_tree(kd_tree, search_point));
//    y_naive.push_back(bench_search_naive(points, search_point));
  }

  line_plot(x, "Dataset Size (number of points)",
            {
                    {y_naive, "Naive"},
                    {y_kdtree, "kD tree"}
            }, "Time to find the nearest point (nanoseconds)",
            "test",
            file_path);
}

int main() {

  synthetic_bench(10000, "../results/test.png");

  return EXIT_SUCCESS;
}