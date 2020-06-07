//
// Created by jackcamp on 6/6/20.
//

#include <iostream>
#include "gnuplot_i.hpp"



float bench_old(std::size_t dataset_size) {}
float bench_new(std::size_t dataset_size) {}

using std::cin;
using std::cout;
using std::endl;

int main() {

  Gnuplot plot("lines");

  plot.set_title("test");
  plot.plot_slope(1.0,0.0,"y=x");

  cout << endl << "Press ENTER to continue..." << endl;
  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
}
