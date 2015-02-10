/******************************************************************************
  Solving Poisson's equation for arbitrary initial conditions 
  by using the finite difference method and Successive Over-Relaxation.

  Inputting initial conditions using a text file
  Calculating the potentials at a series of grid points using SOR
  Finding the electric field from these potentials
  Finding lines of equipotential
  Plotting the potentials as a graphic
  Plotting the electric field as a vector map
  Plotting the lines of equipotential

  Morag Deans, David McLaughlin, Laurynas Mince, Martynas Skirbutas, Mark Wood
*******************************************************************************/

/************************************************************************************************************************************************
                                REFERENCES
  Finite-difference method http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf
  Successive Over-Relaxation http://www.maa.org/publications/periodicals/loci/joma/iterative-methods-for-solving-iaxi-ibi-the-sor-method
  More SOR: http://www.leka.lt/sites/default/files/dokumentai/introductory-finite-difference-methods-for-pdes.pdf
  
*************************************************************************************************************************************************/

#include "main.h"
#include "functions.h"

int main(int argc, char *argv[]) {
  // This is used to extract the cpu usage data at the beginning of the process
  system("./cpu.sh > cpu_start.dat");

  const int grid_rows = atoi(argv[1]);
  const int grid_cols = atoi(argv[2]);
  
  char *initial_condition_file = argv[3];
  
  // We may want to change this later
  const float grid_spacing = 1.0f;

  // Initialise the grid to 0
  float v[grid_rows * grid_cols];
  float new_v[grid_rows * grid_cols];
  for (int element = 0; element < (grid_rows * grid_cols); element++) {
    v[element] = 0;
    new_v[element] = 0;
  }

  // Grab initial conditions from the text file.
  parse(initial_condition_file, v, grid_rows, grid_cols);

  // The voltage at each point is the average of the points around it.
  for (int row = 1; row < (grid_rows - 1); row++) {
    for (int col = 1; col < (grid_cols - 1); col++) {
      v[row * grid_cols + col] = (1/4.0f) * 
        (v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + 
         v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
    }
  }
  
  const int max_iterate = 100000;

  // This is a general formula for calculating the relaxation factor for rectangular grids.
  // NOTE: IT PRODUCES A MORE ACCURATE VALUE (1.93909...) BUT IT SEEMS THAT THIS VALUE INCREASES THE NUMBER OF ITERATIONS. 
  //float relax = 4.0f/(2 + sqrt(4 - (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1))) * (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1)))));
  // THE MOST EFFICIENT VALUE I HAVE FOUND BY TRIAL AND ERROR.
  float relaxation = 1.9f;

  for (int iter = 0; iter < max_iterate; iter++) {
    for (int row = 0; row < grid_rows; row++) {
      for (int col = 1; col < (grid_cols - 1); col++) {
        // For each point in the grid,
        //  V[i,j] = (1-s)V[i,j] + (s/4)(V[i-1,j] + V[i+1,j] + V[i,j-1] + v[i,j+1])
        // where s is the relaxation constant
        // Check reference 3, page 49 for more.
        if (row == 0) {
          //new_v[row * grid_cols + col] = v[(row+1) * grid_cols + col];
          new_v[col] = v[grid_cols + col];

        } else if (row == (grid_rows - 1)) {
          //new_v[row * grid_cols + col] = v[(row - 1) * grid_cols + col];
          new_v[row * grid_cols + col] = v[(r-1) * grid_cols + col];
        } else {
          new_v[row * grid_cols + col] = (1 - relaxation) * 
            v[row * grid_cols + col] + (relaxation / 4) *
             (v[row * grid_cols + (col+1)] +
              new_v[row * grid_cols + (col-1)] + 
              v[(row+1) * grid_cols + col] + 
              new_v[(row-1) * grid_cols + col]); 
        }
      }
    }
    // Find the positions of the shapes between the plates and set
    //  every grid point inside such a shape to 0 by interpreting
    //  the text file. Also set the voltages of the plates back to their
    //  fixed voltages.
    parse(initial_condition_file, v, grid_rows, grid_cols);

    // Check the difference between the elements of the new and the previous matrix. 
    // Act appropriately in case of different errors    
    float err_bound = pow(10, -3);
    float err = error_check(v, new_v, grid_rows, grid_cols);

    // If the new voltage array is close enough to the old one then we stop.
    if (err <= err_bound) {
      std::cout << "The accuracy achieved after " << iter << "th iteration" << "\n";
      break;
    }
    // If we haven't converged but we've run out of iterations then report back
    else if (err > err_bound && iter == max_iterate) {
      std::cout << "Not enough iterations to achieve the required accuracy.\n";
      std::cout << err << "\n";
    }
    else {
      // Set the old matrix equal to the new one ready to go again
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

  // Similarly as before this is used to extract the cpu data at the end of the program
  system("./cpu.sh > cpu_end.dat");
  
  // This calculates the pecentage of CPU used by the program.
  cpu_calc();

  return 0;
}
