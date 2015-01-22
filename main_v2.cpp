#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stdio.h>

using namespace std;

int main(void) {
  // Define a grid.
  // NOTE(david): Do we want these numbers to be variable?
  const int grid_rows = 135;
  const int grid_cols = 230;
  
  // But then why bother huh
  const int grid_spacing = 1;

  // Initialise the grid to 0
  
  // NOTE(david): These are 1D arrays pretending to be 2D arrays.
  //  Indexing them is a nightmare but trust me, everything else is easier.
  //  Index an element by saying v[row * grid_cols + col];
  float v[grid_rows][grid_cols] = {};
  float v_new[grid_rows][grid_cols] = {};

  // Boundary conditions
  // NOTE(david): I've swapped these around so the plates are on the top
  //  and bottom instead of at the sides, since that's closer to the
  //  picture of the problem in the project description
  float top_plate = 1.0f;
  float bottom_plate = -1.0f;

  for (int col = 0; col < grid_cols; col++) {
    // Initial conditions for a capacitor
    v[0][col] = top_plate;
    v[grid_rows - 1][grid_cols] = bottom_plate;
  }

/*********************************************************************
    David's code
**********************************************************************/
  float residuals[grid_rows][grid_cols] = {};
  float err_bound = pow(10, -6);
  
  float t = cos(M_PI / grid_rows) + cos(M_PI / grid_cols);
  float relaxation = (8 - sqrt(64 - 16 * t * t)) / (t * t);
    
  // Obviously this will be different depending on the physical system.
  // The hardest part in the whole program, I imagine, will be filling this.
  float density[grid_rows][grid_cols] = {};
  
  //int stop_condition = false;
  float max_iterate = 500;
  int count = 0;
  while (count < max_iterate) {
    
    // Take the residual of each point in the grid
    // And find the voltage at that point using the residual
    for (int row = 1; row < (grid_rows - 1); row++) {
      for (int col = 0; col < grid_cols; col++) {
        //printf("%f\n", residuals[row][col]);
        
        // NOTE(david): There HAS to be a better way than this...........
        if (col == 0) {
          residuals[row][col] = 
            -1 * v[row][col+1] + v[row][col] + (density[row][col] * grid_spacing * grid_spacing / 
            (8.85f * pow(10,-12)));
        } else if (col == grid_cols) {
          residuals[row][col] = 
            -1 * v[row][col-1] + v[row][col] + (density[row][col] * grid_spacing * grid_spacing / 
            (8.85f * pow(10,-12)));
        } else {
          residuals[row][col] = 
            v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1] - 
            4 * v[row][col] + (density[row][col] * grid_spacing * grid_spacing / 
            (8.85f * pow(10,-12)));
        }
        
        v_new[row][col] = v[row][col] + relaxation * residuals[row][col];
      }
    }
    
    //*v = *v_new;
    
    /*stop_condition = true;
    // if any residual > err_bound, keep going
    for (int row = 0; row < grid_rows; row++) {
      for (int col = 0; col < grid_cols; col++) {
        if (residuals[row][col] > err_bound)  { 
          stop_condition = false;
          break;
        }
      }
    }*/
    count++;
  }    
    
/*********************************************************************
    Laurinas' code
*********************************************************************   
    for (int row = 1; row < (grid_rows - 1); row++) {
      for (int col = 1; col < (grid_cols - 1); col++) {
        v[row][col] = (1/4.0f) * (v[row-1][col] + v[row+1][col] + 
                      v[row][col-1] + v[row][col+1]);
      }
    }

    // TODO(david): Calculate omega from the spacing of the grid?
    float omega = 1.6f;
    
    float err_bound = pow(10, -6);
    
    for (int iter = 0; iter < max_iterate; iter++) {
      for (int row = 1; row < (grid_rows - 1); row++) {
        for (int col = 1; col < (grid_cols - 1); col++) {
	        v[row][col] = (1 - omega) * v[row][col] + (omega / 4) * 
	          (v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
        }
      }
    }
**********************************************************************/

  ofstream output ("potential.dat");

  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      output << setw(15) << v[row][col];
    }
    output << endl;
  }

  output.close();

  return 0;
}
