#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {

  // Define a grid.
  // NOTE: The code could be extended by making a varying grid.
  const int grid_rows = 230;
  const int grid_cols = 230;
  // NOTE(Laurynas): Do we want this to be the case all the time?
  const int grid_spacing = 1;

  // Initialise the grid to 0
  double v[grid_rows][grid_cols] = {0};

  // Boundary conditions
  double top_plate = 100.0;
  double bottom_plate = -100.0;

  for (int col=0; col < grid_cols; col++) {
    // Initial conditions for a capacitor
    // NOTE(Laurynas): I did not initialize a circle at this point since all the elements, except from the boundaries, are 0
    v[0][col] = top_plate;
    v[grid_rows - 1][col] = bottom_plate;
  }

  // Find the mid-point in the grid
  double a = grid_cols/2, b = grid_rows/2;
  // The radius of a circle
  // NOTE(Laurynas): We will probably have to generalise the code to accept some other geometrical figures as well, and will need more user defined parameteres of those figures
  double r = 40;
  // Predefining variables referring to the distance from the centre of a circle squared and the radius squared to find out if an element is within a circle or outside
  double distance_sqrd, r_sqrd = r*r;
  for (int row=1; row < (grid_rows-1); row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
      if (distance_sqrd <= r_sqrd)
	v[row][col] = 0;
      else
	v[row][col] = (1/4.0)*(v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
    }
  }

  // Predifining the relaxation factor s
  // NOTE(Laurynas): there is an equation to calculate this factor for different grids, however it is not a general formula -- it is not valid for all the grids. In general this factor is found by trial and error, and it is between 0 and 2. DO WE WANT TO USE AN EQUATION, OR KEEP THE SAME GRID?

  double s = 1.9729;
  double max_iterate = 50000, err_bound = pow(10, -6);

  // Improving the accuracy of the solution by applying the successive over/under relaxation method
  // NOTE(Laurynas): Need to extend to incorporate error bound
  for (int k=1; k <= max_iterate; k++) {
    for (int row=1; row < (grid_rows-1); row++) {
      for (int col=1; col < (grid_cols-1); col++) {
	// from the equation of a circle
	distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
	// checking if a grid element is inside a circle
	if (distance_sqrd <= r_sqrd)
	  v[row][col] = 0;
	else
	  v[row][col] = (1-s)*v[row][col] + (s/4.0)*(v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
      }
    }
  }

  // file output to record the solution and plot it later
  ofstream output ("potential.dat");

  // Note the reversed order of outputting rows
  for (int row=grid_rows-1; row >= 0; row--) {
    for (int col=0; col<grid_cols; col++) {
      output << setw(15) << v[row][col];
    }
    output << endl;
  }

  output.close();

  return 0;
}
