#include <cmath>
#include "main.h"

void FindElectricField(float v[], int grid_rows, int grid_cols, int grid_spacing) {
  // Initialise the electric field grid
  // E prime is just the grid offset so it lines up with the voltage grid
  double Ex[(grid_rows-1) * (grid_cols-1)];
  double Ey[(grid_rows-1) * (grid_cols-1)];
  double Ex_prime[(grid_rows-1) * (grid_cols-1)];
  double Ey_prime[(grid_rows-1) * (grid_cols-1)];


  //These for statements define the stagered electric field samples
  for (int row = 0; row < (grid_rows-1); row++){
    for (int col = 0; col < (grid_cols-1); col++){
      Ex[row, col]= (v[row, col] - v[(row+1), col]) / grid_spacing;
      Ey[row, col]= (v[row, col] - v[row, (col+1)]) / grid_spacing;
    }
  }

  //This is a fix for the previous electric fields to get a better result
  for (int row = 0; row < (grid_rows - 1); row++){
    for (int col = 0; col < (grid_cols - 1); col++){
      Ex_prime[row, col]= 0.5 *(Ex[row, (col+1)] + Ex[row, col]);
      Ey_prime[row, col]= 0.5 * (Ey[(row+1), col] + Ey[row, col]);
    }
  }

  // outputting the data to file for plotting the ELECTRIC FIELD
  std::ofstream output ("ElectricField.dat");

  // Note the reversed order of outputting rows
  for (int row = 0; row < (grid_rows - 1); row += 3 ) {
    for (int col = 0; col < (grid_cols - 1); col += 3) {
      double x = Ex_prime[row, col];
      double y = Ey_prime[row, col];

      //this is used to normalize the vector(This was Mark's idea so that the 
      //electric field would look better)
      double norm_vector = sqrt(x*x + y*y);
      output << row << " " << col << " " << 
        x/norm_vector << " " << y/norm_vector << "\n";
    }
    output << "\n";
  }
  output.close();
}