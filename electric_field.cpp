#include "main.h"

// Find the electric field given a grid of potentials.

void electric_field(Grid *grid) {
  // Define 1D arrays simulating 2D arrays to store values of electric field
  // We don't want to make these instances of grid; on function exit we're deallocating
  //  a /lot/ of memory -- it causes a segfault.
  float Ex[(grid->rows) * (grid->cols)];
  float Ey[(grid->rows) * (grid->cols)];
  float Ex_prime[(grid->rows) * (grid->cols)];
  float Ey_prime[(grid->rows) * (grid->cols)];

/*
  // I made them instances of Grid...
  Grid Ex((grid->rows), (grid->cols));
  Grid Ey((grid->rows), (grid->cols));
  Grid Ex_prime((grid->rows), (grid->cols));
  Grid Ey_prime((grid->rows), (grid->cols));
*/

  // Find the x- and y-components of the electric field by dividing the difference between the adjacent potentials by the grid spacing
  for (int row = 0; row < grid->rows; row++) {
    for (int col = 0; col < grid->cols; col++) {
      Ex[row * grid->cols + col] = (grid->get(row, col) - grid->get(row, (col+1))) / grid->spacing;
      Ey[row * grid->cols + col] = (grid->get(row, col) - grid->get((row+1), col)) / grid->spacing;
    }
  }

  //This is a fix for the previous electric fields to get a better result
  for (int row = 0; row < grid->rows; row++){
    for (int col = 0; col < grid->cols; col++){
      Ex_prime[row * grid->cols + col] = 0.5 * (Ex[(row+1) * grid->cols + col] + Ex[row * grid->cols + col]);
      Ey_prime[row * grid->cols + col] = 0.5 * (Ey[row * grid->cols + (col+1)] + Ey[row * grid->cols + col]);
    }
  }
  
  // Output to a file
  std::ofstream output ("electric_field.dat");

  // Outputing only one-third of all the array elements to produce a better graphs -- hence row += 3 and col += 3
  for (int row = 0; row < grid->rows; row += 3) {
    for (int col = 0; col < grid->cols; col += 3) {
      float x = Ex_prime[row * grid->cols + col];
      float y = Ey_prime[row * grid->cols + col];
      
      // This is used to normalize the vector (This was Mark's idea so that the electric field would look better)
      float norm_vector = sqrt(x*x + y*y);
      
      // NOTE(Laurynas): Swapped row and col in the first two outputs --
      //  current plot seems correct.
      //  I tend to think that col refers to x-axis and row to y-axis
      output << col << " " << row << " " << x/norm_vector << " " << y/norm_vector << "\n";
    }
    output << "\n";
  }
  output.close();
}
