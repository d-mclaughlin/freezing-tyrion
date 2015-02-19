#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

// Grid is a basic class containing a grid of points
// We should probably take out the function definitions in here and only have
//  declarations. A header file is no place for lengthy defintions.
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
  void set(int row, int col, float value) {
    this->voltages[row * cols + col] = value;
  }
  
  // Evolve takes a given point on the grid and transforms it into the average of the 2 points
  //  from the old grid that are underneath and to the left, and the 2 points from the new grid
  //  that are on top of and to the right, since we've already calculated those.
  //
  // We need the old grid to find the new grid point, and the relaxation factor too.
  // LAURYNAS: VERSION OF EVOLVE WHICH SEEMS TO WORK
  
  
  void evolve(Grid *old, int row, int col, float relaxation) {
    float value;
    
    //Top left and top right corner
    if ((row == 0 && col == 0) || (row == 0 && col == (cols-1))) {
      value = old->get(row+1, col);
    }

    // Bottom left and bottom right corner
    else if ((row == (rows-1) && col == 0) || (row == (rows-1) && col == (cols-1))) {
      value = old->get(row-1, col);
    }

    // Top row
    else if (row == 0) {
      value = old->get(row+1, col);
    }
    // Bottom row
    else if (row == (rows-1)) {
      value = old->get(row-1, col);
    }

    // Left side
    else if (col == 0) {
      value = old->get(row, col+1);
    }
    
    // Right side
    else if (col == (cols-1)) {
     value = old->get(row, col-1);
    }
    
    // Default
    else {
      value = (1 - relaxation) * old->get(row,col) + ( relaxation / 4.0) * 
        (old->get(row, (col+1)) + this->get(row, (col-1)) + this->get((row-1), col) + old->get((row+1), col));
    }
    
    this->set(row, col, value);
  }

  // This is a prime example of something that should be an overloaded = operator
  // Wouldn't be any job to do but I cba right now.
  void operator=(Grid rhs) {
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
	      this->set(row, col, rhs.get(row,col));
      }
    }
  }
  
  float absolute_error(Grid *old) {
    float difference, max = 0.0f;
    
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
	      difference = fabs(this->get(row, col) - old->get(row, col));
	      
	      if (difference > max) {
	        max = difference;
	      }
      }
    }

    return max;
  }

};
#endif
