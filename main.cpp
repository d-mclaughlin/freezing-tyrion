#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stdio.h>

/******************************************************************************
  Solving Poisson's equation using the finite difference method

  Morag Deans, David McLaughlin, Laurynas Mince, Martynas Skirbutas, Mark Wood
*******************************************************************************/

/************************************************************************************************************************************************
                                REFERENCES
  Finite-difference method http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf
  Successive Over-Relaxation http://www.maa.org/publications/periodicals/loci/joma/iterative-methods-for-solving-iaxi-ibi-the-sor-method
  More SOR: http://www.leka.lt/sites/default/files/dokumentai/introductory-finite-difference-methods-for-pdes.pdf
  
*************************************************************************************************************************************************/

using namespace std;

int main(void) {
  // NOTE(david): Make these numbers variable
  const int grid_rows = 4;
  const int grid_cols = 4;  // Temp values; actual values will be larger
  
  const int grid_spacing = 1;

  // Initialise the grid to 0
  float v[grid_rows][grid_cols] = {};

  // Boundary conditions
  // NOTE(david): I've swapped these around so the plates are on the top
  //  and bottom instead of at the sides, since that's closer to the
  //  picture of the problem in the project description
  float top_plate = 1.0f;
  float bottom_plate = -1.0f;

  // TODO(david): Set this up so that it works for any initial conditions
  //  Do we want to do this with a scripting language or do we want to
  //  be able to draw bitmaps? Do we want to get crazy and say both?
  for (int col = 0; col < grid_cols; col++) {
    // Initial conditions for a capacitor
    v[0][col] = top_plate;
    v[grid_rows - 1][col] = bottom_plate;
  }    
  
  // The voltage at each point is the average of the points around it.
  for (int row = 1; row < (grid_rows - 1); row++) {
    for (int col = 1; col < (grid_cols - 1); col++) {
      v[row][col] = (1/4.0f) * (v[row-1][col] + v[row+1][col] + 
                    v[row][col-1] + v[row][col+1]);

      // This misses out the sides of the domain.
      //  The easiest way of fixing this would be to make special cases
      //  for the two sides. Maybe there's a better way though?
    }
  }

  float t = cos(M_PI / grid_rows) + cos(M_PI / grid_cols);
  float relaxation = ((8 - sqrt(64 - (t*t))) / (t*t));
  
  float err_bound = pow(10, -6);
  
  // TODO(david): Swap out max_iterate for some kind of residuals?
  //  I couldn't get it working on 22/1 but it might be the way to go.
  const int max_iterate = 500;

  for (int iter = 0; iter < max_iterate; iter++) {
    for (int row = 1; row < (grid_rows - 1); row++) {
      for (int col = 1; col < (grid_cols - 1); col++) {
        // For each point in the grid,
        //  V[i,j] = (1-s)V[i,j] + (s/4)(V[i-1,j] + V[i+1,j] + V[i,j-1] + v[i,j+1])
        // where s is the relaxation constant
        // Check reference 3, page 49 for more.

        v[row][col] = (1 - relaxation) * v[row][col] + (relaxation / 4) * 
          (v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
        
        // This misses out the sides of the domain.
        //  The easiest way of fixing this would be to make special cases
        //  for the two sides. Maybe there's a better way though?
      }
    }
  }

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
