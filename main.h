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
  ~Grid() {;}

  // Get the value of the grid at a given point
  float get(int row, int col) {
    return this->voltages[row * cols + col];
  }
  
  // Set the value of the grid at a given point to a given value
  void set(int row, int col, int value) {
    this->voltages[row * cols + col] = value;
  }
  
  // Evolve takes a given point on the grid and transforms it into the average of the 2 points
  //  from the old grid that are underneath and to the left, and the 2 points from the new grid
  //  that are on top of and to the right, since we've already calculated those.
  //
  // We need the old grid to find the new grid point, and the relaxation factor too.
  //  x and y are reversed because row is the vertical direction and col is the horizontal direction
  void evolve(Grid *old, int y, int x, float relaxation) {
    // This relies on there being 4 points surrounding this point, which is not always the case
    //  TODO(david): Edge and corner cases
    float value;
    
    // Top row
    /*if (y == 0) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        old->get(x, (y+1)) + this->get((x-1), y) + old->get((x+1), y);
    
    // Bottom row
    } else if (y == (rows - 1)) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        this->get(x, (y-1)) + this->get((x-1), y) + old->get((x+1), y);
    
    // Left column
    } else if (x == 0) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        old->get(x, (y+1)) + this->get(x, (y-1)) + old->get((x+1), y);
    
    // Right column
    } else if (x == (cols - 1)) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        old->get(x, (y+1)) + this->get(x, (y-1)) + old->get((x+1), y);
    
    // Top left corner
    } else if (x == 0 && y == 0) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        old->get(x, (y+1)) + old->get((x+1), y);
     
    // Bottom right corner
    } else if (x == (cols - 1) && y == (rows - 1)) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
         this->get(x, (y-1)) + this->get((x-1), y);
    
    // Top right corner
    } else if (x == (cols - 1) && y == 0) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        old->get(x, (y+1)) + this->get((x-1), y);
    
    // Bottom left corner
    } else if (x == 0 && y == (rows - 1)) {
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        this->get(x, (y-1)) + old->get((x+1), y);
    
    // Default
    } else {*/
      value = (1 - relaxation) * old->get(x,y) + (relaxation / 4) *
        (this->get(y-1,x) + old->get(y+1,x) + this->get(y,x-1) + old->get(y,x+1));
    //}
    
    this->set(y, x, value);
  }
};
#endif
