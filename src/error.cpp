#include "main.h"

int error_check(Grid *old_grid, Grid *new_grid, int error_bound) {

  float maximum = 0, difference;
  // The following loop finds an absolute error between the two arrays that are provided.
  // To avoid storing all the absolute errors and then deciding if the accuracy has been 
  //  achieved I am renewing the variable test_max to find the largest error. 
  // This error is then returned by the function.
  for (int row = 0; row < old_grid->rows; row++) {
    // plates stay at the same potential -- not iterating through them
    for (int col = 1; col < (old_grid->cols - 1); col++) {
      difference = fabs( old_grid->get(row, col) - new_grid->get(row, col) );
      
      // picking the largest error
      if (difference > maximum) {
        maximum = difference;
      }
    }
  }

  return (maximum <= error_bound);
}
