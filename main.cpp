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
  system("./cpu.sh > cpu_start.dat");
  system("./time.sh > time_start.dat");

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
  
  // I've tested this thoroughly and it's all good
  parse(initial_condition_file, &is_fixed, &new_grid);
  equate_matrix(&old_grid, &new_grid);
  
  // Solve the system for a capacitor. In this case the 'relaxation factor' is 1
  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      if (!is_fixed.get(row, col)) {
        new_grid.evolve(&old_grid, row, col, 1);  
      }
    }
  }
  
  std::ofstream output_funtimes("potential.dat");
  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      output_funtimes << new_grid.get(row, col) << " ";
    }
    output_funtimes << "\n";
  }

  // This is a general formula for calculating the relaxation factor for rectangular grids.
  // NOTE: IT PRODUCES A MORE ACCURATE VALUE (1.93909...) BUT IT SEEMS THAT THIS VALUE INCREASES THE NUMBER OF ITERATIONS. 
  //float relax = 4.0f/(2 + sqrt(4 - (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1))) * (cos(PI/(grid_rows-1)) + cos(PI/(grid_cols-1)))));
  // THE MOST EFFICIENT VALUE I HAVE FOUND BY TRIAL AND ERROR.
  float relaxation = 1.9f;
  const int max_iterate = 1;

  for (int iter = 0; iter < max_iterate; iter++) {
    for (int row = 0; row < grid_rows; row++) {
      for (int col = 0; col < grid_rows; col++) {
        // If the value of the fixed grid is zero, i.e. that value is not fixed:
        if (!is_fixed.get(row, col)) {
          // Find the value of this point in the new grid by SOR
          
          // NOTE(david): This is probably where everything goes wrong
          new_grid.evolve(&old_grid, row, col, relaxation);
        }
      }
    }

    // Check the difference between the elements of the new and the previous matrix. 
    // Act appropriately in case of different errors    
    float err_bound = pow(10, -3);

    // If the new voltage array is close enough to the old one then we stop.
    if (error_check(&old_grid, &new_grid, err_bound)) {
      std::cout << "The accuracy achieved after " << iter << "th iteration" << "\n";
      break;
    }
    // If we haven't converged but we've run out of iterations then report back
    else if (iter == max_iterate) {
      std::cout << "Not enough iterations to achieve the required accuracy.\n";
    }
    else {
      // Set the old matrix equal to the new one ready to go again
      equate_matrix(&old_grid, &new_grid);
    }
    
    // The end of the solution
  }

  // Produce a file and store the solution in a form of an array/matrix
  fprint_matrix(&new_grid);

  // Print out the potential values as well as the coordinates: x, y, v(x,y);
  data_equipotential(&new_grid);
  
  // Find the electric field and produce an appropriate data file
  electric_field(&new_grid);

  // Similarly as before this is used to extract the cpu and time data at the end of the program
  system("./cpu.sh > cpu_end.dat");
  system("./time.sh > time_end.dat");
  
  // This calculates the pecentage of CPU used by the program.
  cpu_calc();

  return 0;
}
