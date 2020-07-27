//
// Created by jackcamp on 7/27/20.
//

#include "gnuplot_i.hpp"

#include <iostream>
#include <vector>

struct Labeled_data {
  std::vector<int> data;
  std::string label;
};

void lines(std::vector<int> x, std::string x_axis_title,
          std::vector<Labeled_data> y, std::string y_axis_title,
          std::string title,
          std::string file_path) {

  // Create the plot
  Gnuplot plot("lines");

  // Add descriptors
  plot.set_title(title);
  plot.set_ylabel(y_axis_title);
  plot.set_xlabel(x_axis_title);

  // Add data
  for(auto line : y)
    plot.plot_xy(x, line.data, line.label);

  // Save as an image
  plot.savetofigure(file_path, "png size 1280,720");
  plot.replot();
}

int main() {


  return EXIT_SUCCESS;
}