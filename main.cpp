//
// Created by jackcamp on 6/6/20.
//

#define CGAL_TRACE_STREAM std::cerr

#include "gnuplot_i.hpp"
#include <iostream>

#include "benchmark.h"


int main() {

  std::vector<int> x, yOld, yNew;

  for (int N = 1; N < 2000; N += 1 + (N / 1000)) {

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
  //plot.set_style("fill solid 1.0 noborder");
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
