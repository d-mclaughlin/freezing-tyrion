#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {
  // Define a grid.
  // NOTE(david): Do we want these numbers to be variable?
  const int grid_rows = 135;
  const int grid_cols = 230;
  
  // But then why bother huh
  const int grid_spacing = 1;

  // Initialise the grid to 0
  float v[grid_rows][grid_cols] = {};

  // Boundary conditions
  // NOTE(david): I've swapped these around so the plates are on the top
  //  and bottom instead of at the sides, since that's closer to the
  //  picture of the problem in the project description
  float top_plate = 1.0f;
  float bottom_plate = -1.0f;

  for (int col = 0; col < grid_cols; col++) {
    // Initial conditions for a capacitor
    v[0][col] = top_plate;
    v[grid_rows - 1][col] = bottom_plate;
  }

/* This part should be done recursively no? */

  // Each point is the average of the points around it
  for (int row = 1; row < (grid_rows - 1); row++) {
    for (int col = 1; col < (grid_cols - 1); col++) {
      v[row][col] = (1/4.0f) * (v[row-1][col] + v[row+1][col] + 
                    v[row][col-1] + v[row][col+1]);
    }
  }

  // TODO(david): Calculate omega from the spacing of the grid?
  float omega = 1.6f;
  float max_iterate = 500; 
  float err_bound = pow(10, -6);
  
  for (int iter = 0; iter < max_iterate; iter++) {
    for (int row = 1; row < (grid_rows - 1); row++) {
      for (int col = 1; col < (grid_cols - 1); col++) {
	      v[row][col] = (1 - omega) * v[row][col] + (omega / 4) * 
	        (v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
      }
    }
  }
  
/* End of possibly recursive part */

  ofstream output ("potential.dat");

  for (int row = grid_rows - 1; row >= 0; row++) {
    for (int col = 0; col < grid_cols; col++) {
      output << setw(15) << v[row][col];
    }
    output << endl;
  }

  output.close();

  return 0;
}
