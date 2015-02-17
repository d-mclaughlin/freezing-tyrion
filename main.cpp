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
  // This is used to extract the cpu and time usage data at the beginning of the process
  system("head -1 /proc/stat | grep -o '[0-9]*' > cpu_start.dat");
  system("echo $(date +%s.%N) > time_start.dat");
   
  const int grid_rows = atoi(argv[1]);
  const int grid_cols = atoi(argv[2]);
  
  char *initial_condition_file = argv[3];

  // Initialise three grids:
  //  Old grid is the previous iteration of our grid
  //  New grid is the next iteration of our grid
  //  Is Fixed is the grid deciding whether a point is fixed to a certain voltage

  Grid old_grid(grid_rows, grid_cols);
  Grid new_grid(grid_rows, grid_cols);
  Grid is_fixed(grid_rows, grid_cols);

  // Find which points are fixed and set them to 1 in the fixed grid,
  // and get the inital values of those fixed points and put them in the
  //  old_grid.
  parse(initial_condition_file, &is_fixed, &old_grid);

  new_grid.EquateArray(&old_grid);
 
  /*******************************************
   * Successive over/under relaxation method *
   *******************************************/

  float error_tol = pow(10, -3);
  
  for (int k=0; k<50000; k++) {
    for (int row=0; row < grid_rows; row++) {
      for (int col=0; col < grid_cols; col++) {
	if (is_fixed.voltages[row * grid_cols + col] == 1) {
	  continue;
	}
	else {
	  new_grid.evolve(&old_grid, row, col, 1.9);
	}
      }
    }

    float error = new_grid.AbsoluteError(&old_grid);
    if (error <= error_tol) {
      std::cout << "Accuracy achieved after " << k << "th iteration\n";
      std::cout << "Absolute error is " << error << std::endl;
      break;
    }
    else { 
      old_grid.EquateArray(&new_grid);
    }
  }

  std::ofstream output ("potential_matrix.dat");
  //std::ofstream fixedoutput ("fixed.dat");
  
  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      output << new_grid.voltages[row * grid_cols + col] << " ";
      // fixedoutput << is_fixed.voltages[row * grid_cols + col] << " ";
    }
    output << "\n";
    // fixedoutput << "\n";
  }

  // Similarly as before this is used to extract the cpu and time data at the end of the program
  system("head -1 /proc/stat | grep -o '[0-9]*' > cpu_end.dat");
  system("echo $(date +%s.%N) > time_end.dat");
  
  // This calculates the pecentage of CPU used by the program.
  cpu_calc();
  
  return 0;
}
