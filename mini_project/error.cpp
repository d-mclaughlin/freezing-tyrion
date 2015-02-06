#include "main.h"

float error_check(float old_v[], float new_v[], int grid_rows, int grid_cols, float error_bound) {

  float test_max=0, difference, relative_err;
  // The following loop find an absolute error between the two arrays that are provided. Then if the error corresponds to the very first element of an array (0,0) the variable test_max (i.e. max error) is initialised with this value. If we are not checking the first elements of an array, then the relative error is compared to the previously initialised test value as the first element must have been accessed before. This way the largest relative error is found.
  for (int row=0; row < grid_rows; row++) {
    for(int col=1; col < (grid_cols-1); col++) {
      difference = fabs(old_v[row * grid_cols + col] - new_v[row * grid_cols + col]);
	if (difference > test_max) {
	  test_max = difference;
	}
    }
  }

  return test_max;
}
