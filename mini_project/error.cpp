#include "main.h"

float error_check(float old_v[], float new_v[], int grid_rows, int grid_cols, float error_bound) {

  float test_max=0, difference, relative_err;
  // The following loop finds an absolute error between the two arrays that are provided. To avoid storing all the absolute errors and then deciding if the accuracy has been achieved I am renewing the variable test_max to find the largest error. This error is then returned by the function.
  for (int row=0; row < grid_rows; row++) {
    // plates stay at the same potential -- not iterating through them
    for(int col=1; col < (grid_cols-1); col++) {
      difference = fabs(old_v[row * grid_cols + col] - new_v[row * grid_cols + col]);
      // picking the lasrgest error;
	if (difference > test_max) {
	  test_max = difference;
	}
    }
  }

  return test_max;
}
