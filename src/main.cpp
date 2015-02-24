#include "main.h"
#include "functions.h"

int main(int argc, char *argv[]) {
  // Get the start time of the program
  system("echo $(date +%s.%N) > misc/time_start.dat");

  // Extract the cpu usage data a the begining of the process
  system("head -1 /proc/stat | grep -o '[0-9]*' > misc/cpu_start.dat");

  const int grid_rows = atoi(argv[1]);
  const int grid_cols = atoi(argv[2]);
  
  char *initial_condition_file = argv[3];
  
  // Initialise three grids:
  // Old grid is the previous iteration of our rid
  // New grid is the next iteration of our grid
  // Is Fixed is the grid deciding whether a point is fixed to a certain voltage
  
  Grid old_grid(grid_rows, grid_cols);
  Grid new_grid(grid_rows, grid_cols);
  Grid is_fixed(grid_rows, grid_cols);
  
  // Find which points are fixed and set them to 1 in the fixed grid,
  // and get the inital values of those fixed points and put them in the
  // old_grid.
  
  parse(initial_condition_file, &is_fixed, &old_grid);
  new_grid = old_grid;
  
  /*******************************************
   * Successive over/under relaxation method *
   *******************************************/
  
  float error_tol = pow(10, -3);
  int max_iter = 50000;
  
  for (int iter = 0; iter < max_iter; iter++) {
    for (int row = 0; row < grid_rows; row++) {
      for (int col = 0; col < grid_cols; col++) {
	      // If this point on the grid is not fixed, ie is_fixed == 0
	      if (!is_fixed.get(row, col)) {
	        new_grid.evolve(&old_grid, row, col, 1.9);
	      }
      }
    }
    
    float error = new_grid.absolute_error(&old_grid);
    if (error <= error_tol) {
      std::cout << "Accuracy achieved after " << iter << "th iteration\n";
      std::cout << "Absolute error is " << error << std::endl;
      break;
    } else if (iter == max_iter) {
      std::cout << "Accuracy achieved after " << iter << "th iteration\n";
      std::cout << "Absolute error is " << error << std::endl;
    } else {
      old_grid = new_grid;
    }
  }

  print_grid_to_file("misc/potential_matrix.dat", &new_grid, 0);

  // Print out the potential values as well as the coordinates: x, y, v(x,y);
  print_grid_to_file("misc/equipotential.dat", &new_grid, 1);

  // Find the electric field and produce an appropriate data file
  electric_field(&new_grid);

  // Extract the cpu data at the end of the program
  system("head -1 /proc/stat | grep -o '[0-9]*' > misc/cpu_end.dat");

  // Get the end time of the program
  system("echo $(date +%s.%N) > misc/time_end.dat");
  
  // Put the value of the ram usage at the end of the program into the file
  system("pmap $(ps | grep main.exe | grep -o '[0-9]*' | head -1) | grep -o '[0-9]*' | tail -1 > misc/memory.dat");

  // Calculate the percentage of CPU used by the program.
  cpu_calc();

  return 0;
}
