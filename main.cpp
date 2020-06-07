//
// Created by jackcamp on 6/6/20.
//

#include <iostream>
#include "gnuplot_i.hpp"



double bench_old(std::size_t dataset_size) {
  return dataset_size;
}

double bench_new(std::size_t dataset_size) {
  return dataset_size * 2;
}

using std::cin;
using std::cout;
using std::endl;

int main() {

  std::vector<double> x, yOld, yNew;

  for (std::size_t i = 0; i < 10000; ++i) {

    x.push_back(i);
    yOld.push_back(bench_old(i));
    yNew.push_back(bench_new(i));
  }


  Gnuplot plot("lines");

  plot.set_title("test");
  plot.plot_xy(x, yOld, "Old");
  plot.plot_xy(x, yNew, "New");


  cout << endl << "Press ENTER to continue..." << endl;
  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
}
