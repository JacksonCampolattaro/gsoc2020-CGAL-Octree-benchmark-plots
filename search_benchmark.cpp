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

typedef CGAL::Octree::Octree<Point_set, typename Point_set::Point_map> Octree;

typedef std::chrono::nanoseconds time_unit;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;


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

  std::cout << "..... " << naive_nearest << std::endl;
  return duration_cast<time_unit>(end - start).count();
}


void synthetic_bench(std::size_t num_points, std::string file_path) {

  std::vector<int> x, y_naive;

  for (std::size_t N = 0; N < num_points; ++N) {

    // Set N
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

    // Perform
    y_naive.push_back(bench_search_naive(points, search_point));
  }

  line_plot(x, "Dataset Size (number of points)",
            {
                    {y_naive, "Naive"}
            }, "Time to find the nearest point (nanoseconds)",
            "test",
            file_path);
}

int main() {

  synthetic_bench(10000, "../results/test.png");

  return EXIT_SUCCESS;
}