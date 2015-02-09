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

// Function prototypes
void parse(char *filename, float v[], int grid_rows, int grid_cols);
void electric_field(float v[], int grid_rows, int grid_cols, float grid_spacing);
void cpu_calc(void);

int main(int argc, char *argv[]) {
  //This is used to extract the cpu usage data at the beginning of the process
  system("./cpu.sh > cpu_start.dat");

  const int grid_rows = atoi(argv[1]);
  const int grid_cols = atoi(argv[2]);
  
  char *initial_condition_file = argv[3];
  
  // We may want to change this later
  const float grid_spacing = 1.0f;

  // Initialise the grid to 0
  float v[grid_rows * grid_cols];
  for (int element = 0; element < (grid_rows * grid_cols); element++) {
    v[element] = 0;
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

  // This doesn't produce the right value for the relaxation factor...
  float t = cos(M_PI / grid_rows) + cos(M_PI / grid_cols);
  float relaxation = ((8 - sqrt(64 - (t*t))) / (t*t));

  // ...but this value makes the calculations blow up exponentially
  //float relaxation = 1.97f;
  
  //float err_bound = pow(10, -6);

  // TODO(david): Swap out max_iterate for some kind of residuals?
  //  I couldn't get it working on 22/1 but it might be the way to go.
  const int max_iterate = 50000;

  for (int iter = 0; iter < max_iterate; iter++) {
    for (int row = 0; row < grid_rows; row++) {
      for (int col = 1; col < (grid_cols - 1); col++) {
        // For each point in the grid,
        //  V[i,j] = (1-s)V[i,j] + (s/4)(V[i-1,j] + V[i+1,j] + V[i,j-1] + v[i,j+1])
        // where s is the relaxation constant
        // Check reference 3, page 49 for more.
        if (row == 0) {
          v[row * grid_cols + col] = (1 - relaxation) * 
            v[row * grid_cols + col] + (relaxation / 4) *  
            (v[(row+1) * grid_cols + col] +
             v[row * grid_cols + (col-1)] +  
             v[row * grid_cols + (col+1)]);
        } else if (row == (grid_rows - 1)) {
          v[row * grid_cols + col] = (1 - relaxation) * 
            v[row * grid_cols + col] + (relaxation / 4) *  
            (v[(row-1) * grid_cols + col] + 
             v[row * grid_cols + (col-1)] + 
             v[row * grid_cols + (col+1)]);
        } else {
          v[row * grid_cols + col] = (1 - relaxation) * 
            v[row * grid_cols + col] + (relaxation / 4) *
            (v[(row-1) * grid_cols + col] + 
             v[(row+1) * grid_cols + col] + 
             v[row * grid_cols + (col-1)] + 
             v[row * grid_cols + (col+1)]);
        }
      }
    }
    // Find the positions of the shapes between the plates and set
    //  every grid point inside such a shape to 0 by interpreting
    //  the text file. Also set the voltages of the plates back to their
    //  fixed voltages.
    parse(initial_condition_file, v, grid_rows, grid_cols);
  }

  std::ofstream potential_output ("potential.dat");
  std::ofstream matrix_output ("matrix_potential.dat");

  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      potential_output << v[row * grid_cols + col] << " ";
      // NOTE(david): This no longer works since i reversed the order of outputting the rows.
      matrix_output << v[row * grid_cols + col] << " ";
    }
    potential_output << "\n";
    matrix_output << "\n";
  }

  potential_output.close();
  matrix_output.close();

  electric_field(v, grid_rows, grid_cols, grid_spacing);

  // Similarly as before this is used to extract the cpu data at the end of the program
  system("./cpu.sh > cpu_end.dat");
  
  // This calculates the pecentage of CPU used by the program.
  cpu_calc();

  return 0;
}
