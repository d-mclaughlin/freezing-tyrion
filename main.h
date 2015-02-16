#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

// Grid is a basic class containing a grid of points
class Grid {
public:
  std::vector<float> voltages;
  int rows;
  int cols;
  float spacing;

  // Constructor
  Grid(int grid_rows, int grid_cols) {
    rows = grid_rows;
    cols = grid_cols;
    
    // We may want to change this later
    spacing = 1.0f;

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

  // Get the value of the grid at a given point
  float get(int row, int col) {
    return this->voltages[row * cols + col];
  }
  
  // Set the value of the grid at a given point to a given value
  void set(int row, int col, int value) {
    this->voltages[row * cols + col] = value;
  }
  
  // Evolve takes a given point on the grid and transforms it into the average of the 4 points around it
  // We need the old grid to find the new grid point, and the relaxation factor too.
  void evolve(Grid *old, int x, int y, float relaxation) {
    // This relies on there being 4 points surrounding this point, which is not always the case
    //  TODO(david): Edge and corner cases
    this->set(x, y, (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
      old->get(x, (y+1)) + old->get(x, (y-1)) + old->get((x-1), y) + old->get((x-1), y));
  }
};
#endif
