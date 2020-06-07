//
// Created by jackcamp on 6/6/20.
//

#include "gnuplot_i.hpp"
#include <iostream>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;

using namespace std::chrono;

size_t bench_old(std::size_t dataset_size) {

  auto start = high_resolution_clock::now();

  // Code to be timed

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start).count();

  return duration;
}

size_t bench_new(std::size_t dataset_size) {

  auto start = high_resolution_clock::now();

  // Code to be timed

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start).count();

  return duration;
}

int main() {

  std::vector<size_t> x, yOld, yNew;

  for (size_t i = 0; i < 10000; ++i) {

    x.push_back(i);
    yOld.push_back(bench_old(i));
    yNew.push_back(bench_new(i));
  }


  Gnuplot plot("lines");
  plot.set_title("test");
  plot.plot_xy(x, yOld, "Old");
  plot.plot_xy(x, yNew, "New");


  cout << endl << "Press ENTER to continue..." << endl;
  cin.clear();
  cin.ignore(cin.rdbuf()->in_avail());
  cin.get();
}
