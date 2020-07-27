//
// Created by jackcamp on 7/27/20.
//

#include <iostream>
#include <vector>

struct Labeled_data {
  std::vector<int> data;
  std::string label;
};

void plot(Labeled_data x,
          std::vector<Labeled_data> y,
          std::string file_path) {
//
//  // Create the plot
//  Gnuplot plot("lines");
//
//  // Add descriptors
//  plot.set_title("Comparison of Algorithms for Constructing an Octree");
//  plot.set_ylabel("Time to Build a Tree (Microseconds)");
//  plot.set_xlabel("Number of Points Added");
//
//  // Add data
//  plot.plot_xy(x, y_old, "Old");
//  plot.plot_xy(x, y_new, "New");
//  plot.plot_xy(x, y_improved, "Improved");
//
//  // Save as an image
//  plot.savetofigure(file_path, "png size 1280,720");
//  plot.replot();
//
}

int main() {


  return EXIT_SUCCESS;
}