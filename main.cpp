#include "main.h"
#include "functions.h"

int main(int argc, char *argv[]) {


  //This gets the process ID number
  system("ps ux |grep main.exe | grep -o '[0-9]*' | head -1 > PID.dat");

  //This gets the start time of the program
  system("echo $(date +%s.%N) > time_start.dat");

//This is used to extract the cpu usage data a the begining of the process
  system("head -1 /proc/stat | grep -o '[0-9]*' > cpu_start.dat");

  //Putting the value of the ram usage at the start of he pogram ino the file
  system("./ram.sh PID.dat >> memory.dat");





  const int grid_rows = atoi(argv[1]);
  const int grid_cols = atoi(argv[2]);
  
  char *initial_condition_file = argv[3];

  // Initialise three grids:
  //  Old grid is the previous iteration of our rid
  //  New grid is the next iteration of our grid
  //  Is Fixed is the grid deciding whether a point is fixed to a certain voltage

  Grid old_grid(grid_rows, grid_cols);
  Grid new_grid(grid_rows, grid_cols);
  Grid is_fixed(grid_rows, grid_cols);

  // Find which points are fixed and set them to 1 in the fixed grid,
  // and get the inital values of those fixed points and put them in the
  //  old_grid.
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
  
  // Output what the grid looks like after the initial set up. Looks fine to me!
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
  const int max_iterate = 10000;

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
/*
    // If the new voltage array is close enough to the old one then we stop.
    if (error_check(&old_grid, &new_grid, err_bound)) {
      std::cout << "The accuracy achieved after " << iter << "th iteration" << "\n";
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
*/
    equate_matrix(&old_grid, &new_grid);    
    // The end of the solution
  }
    
  // Produce a file and store the solution in a form of an array/matrix

  print_grid_to_file("potential_matrix.dat", &new_grid, 0);

  // Print out the potential values as well as the coordinates: x, y, v(x,y);
  print_grid_to_file("equipotential.dat", &new_grid, 1);

  
  // Find the electric field and produce an appropriate data file
  electric_field(&new_grid);
  





  //Similarly as before this is used to extract the cpu data at the end of the program
  system("head -1 /proc/stat | grep -o '[0-9]*' > cpu_end.dat");

 //This gets the end time of the program
  system("echo $(date +%s.%N) > time_end.dat");
  
 //Putting the value of the ram usage at the end of he pogram ino the file
  system("./ram.sh PID.dat >> memory.dat");

  //This calculates he pecentage of CPU used by the program.
  cpu_calc();



  return 0;
}
