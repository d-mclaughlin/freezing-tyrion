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
  /*
  void evolve(Grid *old, int x, int y, float relaxation) {
    // This relies on there being 4 points surrounding this point, which is not always the case
    //  TODO(david): Edge and corner cases
    this->set(x, y, (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
      old->get(x, (y+1)) + old->get(x, (y-1)) + old->get((x-1), y) + old->get((x+1), y));
  }
  */

  // LAURYNAS: VERSION OF EVOLVE WHICH SEEMS TO WORK
  void evolve(Grid *old, Grid *is_fixed, int row, int col, float relaxation) {
    if (is_fixed->voltages[row * cols + col] == 1) {
      return;
    }
    // Top left corner
    else if (row == 0 && col == 0) {
      this->voltages[row * cols + col] = old->voltages[(row+1) * cols + col];
    }
    // Top right corner
    else if (row == 0 && col == (cols-1)) {
      this->voltages[row * cols + col] = old->voltages[(row+1) * cols + col];
    }
    // Bottom left corner
    else if (row == (rows-1) && col == 0) {
      this->voltages[row * cols + col] = old->voltages[(row-1) * cols + col];
    }
    // Bottom right corner
    else if (row == (rows-1) && col == (cols-1)) {
      this->voltages[row * cols + col] = old->voltages[(row-1) * cols + col];
    }
    // Top row
    else if (row == 0) {
      this->voltages[row * cols + col] = old->voltages[(row+1) * cols + col];
    }
    else if (row == 0) {
      this->voltages[row * cols + col] = old->voltages[(row+1) * cols + col];
    }
    // Bottom row
    else if (row == (rows-1)) {
      this->voltages[row * cols + col] = old->voltages[(row-1) * cols + col];
    }
    else if (row == (rows-1)) {
      this->voltages[row * cols + col] = old->voltages[(row-1) * cols + col];
    }
    // Left side
    else if (col == 0) {
      this->voltages[row * cols + col] = old->voltages[row * cols + (col+1)];
    }
    // Right side
    else if (col == (cols-1)) {
      this->voltages[row * cols + col] = old->voltages[row * cols + (col-1)];
    }
    else {
      this->voltages[row * cols + col] = (1 - relaxation) * old->get(row,col) + ( relaxation / 4.0) * (old->get(row, (col+1)) + this->get(row, (col-1)) + this->get((row-1), col) + old->get((row+1), col));
    }
  }
  
};
#endif
