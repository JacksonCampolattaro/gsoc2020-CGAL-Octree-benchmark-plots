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

#define SAMPLES_PER_TEST 100000

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

  std::cout << "naive --> " << naive_nearest << "\r";
  return duration_cast<time_unit>(end - start).count();
}

int bench_search_kd_tree(Kd_tree &kd_tree, Point search_point, size_t k) {

  auto start = high_resolution_clock::now();

  Kd_tree_search search(kd_tree, search_point, k);

  auto end = high_resolution_clock::now();

  for (auto p : search)
    std::cout << "kd_tree --> " << p.first << "\r";
  return duration_cast<time_unit>(end - start).count();
}

int bench_search_octree(Octree &octree, Point search_point, size_t k) {

  std::vector<Point> nearest_neighbors;

  auto start = high_resolution_clock::now();

  octree.nearest_k_neighbors(search_point, k, std::back_inserter(nearest_neighbors));

  auto end = high_resolution_clock::now();

  for (auto p : nearest_neighbors)
    std::cout << "octree --> " << p << "\r";
  return duration_cast<time_unit>(end - start).count();
}


void search_time_vs_point_count(size_t num_points, std::string file_path) {

  std::vector<int> x, y_naive, y_kdtree, y_octree;

  for (std::size_t N = 1; N < num_points; ++N) {

    x.push_back(N);
    std::cout << N << std::endl;

    // Generate a dataset
    Point_set points;
    CGAL::Random_points_on_sphere_3<Point> generator;
    points.reserve(N);
    for (std::size_t i = 0; i < N; ++i)
      points.insert(*(generator++));

    // Build the kd_tree
    auto kdtree_points_copy = points;
    Kd_tree kd_tree(kdtree_points_copy.points().begin(), kdtree_points_copy.points().end());
    kd_tree.build();

    // Build the octree
    auto octree_points_copy = points;
    auto point_map = octree_points_copy.point_map();
    Octree octree(octree_points_copy, point_map);
    octree.refine(10, 20);

    // Time results will be put here
    int naive_time, kdtree_time, octree_time;
    naive_time = kdtree_time = octree_time = 0;

    // Repeat the benchmark to get higher quality results
    for (int i = 0; i < SAMPLES_PER_TEST; ++i) {

      // Generate a search point
      Point search_point = *generator++;

      naive_time += bench_search_naive(points, search_point);

      kdtree_time += bench_search_kd_tree(kd_tree, search_point, 1);

      octree_time += bench_search_octree(octree, search_point, 1);
    }

    y_naive.push_back(naive_time / SAMPLES_PER_TEST);
    y_kdtree.push_back(kdtree_time / SAMPLES_PER_TEST);
    y_octree.push_back(octree_time / SAMPLES_PER_TEST);

  }

  line_plot(x, "Dataset Size (number of points)",
            {
                    {y_naive,  "Naive"},
                    {y_kdtree, "kD tree"},
                    {y_octree, "Octree"}
            }, "Time to find the nearest point (nanoseconds)",
            "Comparison of Algorithms for Finding the Nearest Neighbor of a Point",
            file_path);
}

void search_time_vs_k(size_t num_points, size_t max_k, std::string file_path) {

  std::vector<int> x, y_kdtree, y_octree;

  // Generate a dataset
  Point_set points;
  CGAL::Random_points_on_sphere_3<Point> generator;
  points.reserve(num_points);
  for (std::size_t i = 0; i < num_points; ++i)
    points.insert(*(generator++));

  // Build the kd_tree
  auto kdtree_points_copy = points;
  Kd_tree kd_tree(kdtree_points_copy.points().begin(), kdtree_points_copy.points().end());
  kd_tree.build();

  // Build the octree
  auto octree_points_copy = points;
  auto point_map = octree_points_copy.point_map();
  Octree octree(octree_points_copy, point_map);
  octree.refine(10, 20);

  // Try different values of k
  for (size_t k = 1; k < max_k; ++k) {

    x.push_back(k);
    std::cout << k << std::endl;

    // Time results will be put here
    int kdtree_time, octree_time;
    kdtree_time = octree_time = 0;

    // Repeat the benchmark to get higher quality results
    for (int i = 0; i < SAMPLES_PER_TEST; ++i) {

      // Generate a search point
      Point search_point = *generator++;

      kdtree_time += bench_search_kd_tree(kd_tree, search_point, k);

      octree_time += bench_search_octree(octree, search_point, k);
    }

    y_kdtree.push_back(kdtree_time / SAMPLES_PER_TEST);
    y_octree.push_back(octree_time / SAMPLES_PER_TEST);
  }

  line_plot(x, "K (Number of Neighbors to find)",
            {
                    {y_kdtree, "kD tree"},
                    {y_octree, "Octree"}
            }, "Time to find neighbors (nanoseconds)",
            "Time to Find K Nearest Neighbors in a Set of " + std::to_string(num_points) + " Points",
            file_path);
}


int main() {

  search_time_vs_point_count(1000, "../results/test.png");
//  search_time_vs_k(10000, 1000, "../results/test.png");

  return EXIT_SUCCESS;
}