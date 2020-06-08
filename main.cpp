//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include "gnuplot_i.hpp"
#include <iostream>

#include "benchmark.h"

#define SAMPLES_PER_TEST 100

void synthetic_bench() {

  std::vector<int> x, yOld, yNew;

  for (int N = 1; N < 2000; N += 1 + (N / 100)) {

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

void photogrammetry_bench() {

  // Read example data from file
  std::ifstream stream("../data/archer_original.ply");
  Point_set points;
  stream >> points;
}

int main() {

  synthetic_bench();
}
