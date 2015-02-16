#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

// Grid is a basic class containing a grid of points, and a way of getting a point 
//  given a row and column number
class Grid {
private:
  int rows;
  int cols;

public:
  std::vector<float> voltages;
  float grid_spacing;

  // Initializer
  Grid(int grid_rows, int grid_cols) {
    rows = grid_rows;
    cols = grid_cols;

    // We may want to change this later
    grid_spacing = 1.0f;

    voltages.resize(rows * cols);

    // Initilase voltages vector to 0
    for (int element = 0; element < rows * cols; element++) {
      voltages[element] = 0;
    }
  }

  // Destructor
  ~Grid() {
    delete this;
  }

  // Evolve takes a given point on the grid and transforms it into the average of the 4 points around it
  // We need the old grid to find the new grid point, and the relaxation factor too.
  void evolve(Grid *old, int x, int y, float relaxation) {
    this->voltages[x * cols + y] = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
      old->get(x, (y+1)) + old->get(x, (y-1)) + old->get((x-1), y) + old->get((x-1), y);
  }

  // Get
  float get(int row, int col) {
    return this->voltages[row * cols + col];
  }
};

#endif
