#include "main.h"
#include "functions.h"

int main(void) {
  //Define a grid. Grid has now a constant size which can be changed by editing the code -- this could be extended to a user defined grid size
  const int grid_rows = 100;
  const int grid_cols = 100;
  const int grid_spacing = 1;

  float v[grid_rows * grid_cols] = { 0 };
  float new_v[grid_rows * grid_cols] = { 0 };

  // Boundary Conditions
  float left_plate = 2.5;
  float right_plate = -2.5;
 
  for (int row=0; row < grid_rows; row++) {
    v[row * grid_cols] = left_plate;
    v[row * grid_cols + (grid_cols - 1)] = right_plate;
    new_v[row * grid_cols] = left_plate;
    new_v[row * grid_cols + (grid_cols - 1)] = right_plate;
  }

  /*****************************************************************
   * Solution to the problem using the Finite Differece Method     *
   *                                                               *
   *****************************************************************/

  // First, iterate through v to find initial solution to the problem, then keep iterating to correct the previous value until the required accuracy is achieved

  /*********************************************************/
  // Parameters of a circle
  float r = 20.0f; // radius
  float a = grid_cols/2.0, b = grid_rows/2.0; // Centre of the circle
  float distance_sqrd, r_sqrd = r*r;
  /********************************************************/

  for (int row=1; row < (grid_rows-1); row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
      // If the grid element is in the area of a circle then its value is 0
      if (distance_sqrd <= r_sqrd)
	v[row * grid_cols + col] = 0;
      else // general case
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
    }
  }

  // Further iteration to increase accuracy
  int max_iterate = 100000;
  float err_bound = pow(10, -3);
  const float PI = 3.141592654f;
  // This is a general formula for calculating the relaxation factor for rectangular grids.
  // NOTE: IT PRODUCES A MORE ACCURATE VALUE (1.93909...) BUT IT SEEMS THAT THIS VALUE INCREASES THE NUMBER OF ITERATIONS. 
  //float relax = 4.0f/(2 + sqrt(4 - (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1))) * (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1)))));
  // THE MOST EFFICIENT VALUE I HAVE FOUND BY TRIAL AND ERROR. TWICE AS EFFICIENT AS THE ABOVE VALUE! 
  float relax = 1.9f;
  for (int k=1; k <= max_iterate; k++) {
    if (k%1000==0)
      cout << "Currently on the " << k << "th iteration" << endl;
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < (grid_cols - 1); col++) {
	distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
	// Reinitialise circle
	if (distance_sqrd <= r_sqrd)
	  new_v[row * grid_cols + col] = 0;
	// Equate the top edge to the next row
	else if (row == 0)
	  new_v[col] = v[grid_cols + col];
	// Equate the bottom edge to the previous row
	else if (row == (grid_rows - 1)) // similar to the above
	  new_v[row * grid_cols + col] = v[(row-1) * grid_cols + col];
	else // general case
	  new_v[row * grid_cols + col] = (1 - relax) * v[row*grid_cols + col] + (relax/4.0) * (v[row * grid_cols + (col+1)] + new_v[row * grid_cols + (col-1)] + v[(row+1) * grid_cols + col] + new_v[(row-1) * grid_cols + col]);
      }
    }
    
    // Check the difference between the elements of the new and the previous matrix. Act appropriatly in case of different errors    
    float err = error_check(v, new_v, grid_rows, grid_cols, err_bound);
    //    cout << err << endl;
    if (err <= err_bound) {
      cout << "The accuracy achieved after " << k << "th iteration" << endl;
      break;
    }
    else if (err > err_bound && k == max_iterate) {
      cout << "Not enough iterations to achieve the required accuracy." << endl;
      cout << err << endl;
    }
    else {
      // Renew the values of the array v by equating it to the array new_v
      equate_matrix(v, new_v, grid_rows, grid_cols);
    }
    
    // The end of the solution
  }
    
  // Produce a file and store the solution in a form of an array/matrix
  fprint_matrix(new_v, grid_rows, grid_cols);

  // Print out the potential values as well as the coordinates: x, y, v(x,y);
  data_equipotential(new_v, grid_rows, grid_cols);

  
  // Find the electric field and produce an appropriate data file
  electric_field(new_v, grid_rows, grid_cols, grid_spacing);
  
  return 0;
}
