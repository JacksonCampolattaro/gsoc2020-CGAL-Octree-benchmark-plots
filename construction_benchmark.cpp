//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include <CGAL/point_generators_3.h>
#include <CGAL/Point_set_3/IO.h>
#include <CGAL/random_simplify_point_set.h>

#include "plotting.h"
#include <iostream>
#include <istream>

#include "benchmark.h"

#define SAMPLES_PER_TEST 10

void synthetic_bench(int max_points, std::string file_path) {

  std::vector<int> x, y_old, y_new, y_improved;

  for (int N = 1; N < max_points; N += 1 + (N / 20)) {

    cout << N << endl;
    x.push_back(N);

    int oldResult = 0;
    int newResult = 0;
    int improvedResult = 0;

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

    }

    y_old.push_back(oldResult / SAMPLES_PER_TEST);
    y_new.push_back(newResult / SAMPLES_PER_TEST);
    y_improved.push_back(improvedResult / SAMPLES_PER_TEST);

  }

  line_plot(x, "Number of Points Added",
            {
                    {y_old,      "Old"},
                    {y_new,      "New"},
                    {y_improved, "Improved"}
            }, "Time to Build a Tree (Microseconds)",
            "Comparison of Algorithms for Constructing an Octree",
            file_path
  );
}

void photogrammetry_bench(int max_points, std::string file_path) {

  std::vector<int> x, y_old, y_new, y_improved;

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
  }

  line_plot(x, "Number of Points Added",
            {
                    {y_old,      "Old"},
                    {y_new,      "New"},
                    {y_improved, "Improved"}
            }, "Time to Build a Tree (Microseconds)",
            "Comparison of Algorithms for Constructing an Octree",
            file_path
  );
}

int main() {

  synthetic_bench(
          2000,
          "../results/octree-benchmark-plot-spherical_shell-release_mode-2000_points.png");
//  synthetic_bench(
//          10000,
//          "../results/octree-benchmark-plot-spherical_shell-release_mode-10000_points.png");
//  synthetic_bench(
//          1000000,
//          "../results/octree-benchmark-plot-spherical_shell-release_mode-1000000_points.png");

//  photogrammetry_bench(
//          2000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-2000_points.png");
//  photogrammetry_bench(
//          10000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-10000_points.png");
//  photogrammetry_bench(
//          1000000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-1000000_points.png");
}
