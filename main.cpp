//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include <CGAL/point_generators_3.h>
#include <CGAL/Point_set_3/IO.h>
#include <CGAL/random_simplify_point_set.h>

#include "gnuplot_i.hpp"
#include <iostream>
#include <istream>

#include "benchmark.h"

#define SAMPLES_PER_TEST 100

void plot(std::vector<int> x, std::vector<int> y_old, std::vector<int> y_new, std::string file_path) {

  // Create the plot
  Gnuplot plot("lines");

  // Add descriptors
  plot.set_title("Comparison of Old and New Algorithms for Constructing an Octree");
  plot.set_ylabel("Time to Build a Tree (Microseconds)");
  plot.set_xlabel("Number of Points Added");

  // Add data
  plot.plot_xy(x, y_old, "Old");
  plot.plot_xy(x, y_new, "New");

  // Save as an image
  plot.savetofigure(file_path, "png size 1280,720");
  plot.replot();

}

void synthetic_bench(int max_points, std::string file_path) {

  std::vector<int> x, yOld, yNew;

  for (int N = 1; N < max_points; N += 1 + (N / 100)) {

    cout << N << endl;
    x.push_back(N);

    int oldResult = 0;
    int newResult = 0;

    for (int sample = 0; sample < SAMPLES_PER_TEST; ++sample) {

      // Generate random point set
      Point_set points;
      CGAL::Random_points_on_sphere_3<Point> generator;
      points.reserve(N);
      for (std::size_t i = 0; i < N; ++i)
        points.insert(*(generator++));

      oldResult += bench_old(points);
      newResult += bench_new(points);

    }

    yOld.push_back(oldResult / SAMPLES_PER_TEST);
    yNew.push_back(newResult / SAMPLES_PER_TEST);

  }

  plot(x, yOld, yNew, file_path);
}

void photogrammetry_bench(int max_points, std::string file_path) {

  std::vector<int> x, yOld, yNew;

  // Read example data from file
  std::ifstream stream("../data/archer_original.ply");
  Point_set points;
  stream >> points;

  double percent_to_remove = 100 - (100.0 * max_points / points.number_of_points());
  points.remove(CGAL::random_simplify_point_set(points, percent_to_remove), points.end());

  while (points.number_of_points() > 10) {

    cout << points.size() << endl;

    points.remove(CGAL::random_simplify_point_set(points, 0.01) - 1, points.end());

    x.insert(x.begin(), points.size());
    yOld.insert(yOld.begin(), bench_old(points));
    yNew.insert(yNew.begin(), bench_new(points));
  }

  plot(x, yOld, yNew, file_path);
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

  photogrammetry_bench(
          2000,
          "../results/octree-benchmark-plot-statue_scan-release_mode-2000_points.png");
//  photogrammetry_bench(
//          10000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-10000_points.png");
//  photogrammetry_bench(
//          1000000,
//          "../results/octree-benchmark-plot-statue_scan-release_mode-1000000_points.png");
}
