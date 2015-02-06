#include "main.h"

void electric_field(float v[], int grid_rows, int grid_cols, float grid_spacing) {

  // Defining and initializing constants
  //const float grid_spacing = 1;
  //const int grid_rows = 100;
  //const int grid_cols = 100;

  // Define 1D arrays simulating 2D arrays to store values of electric field
  float Ex[(grid_rows) * (grid_cols)];
  float Ey[(grid_rows) * (grid_cols)];
  float Ex_prime[(grid_rows) * (grid_cols)];
  float Ey_prime[(grid_rows) * (grid_cols)];
  
  // Find the x- and y-components of the electric field by dividing the difference between the adjacent potentials by the grid spacing
  for (int row=0; row < (grid_rows); row++) {
    for (int col=0; col < (grid_cols);col++) {
      Ex[row * grid_cols + col]= (v[row * grid_cols + col] - v[row * grid_cols + (col+1)]) / grid_spacing;
      Ey[row * grid_cols + col]= (v[row * grid_cols + col] - v[(row+1) * grid_cols + col]) / grid_spacing;
    }
  }

  //This is a fix for the previous electric fields to get a better result
  for (int row=0; row < (grid_rows); row++){
    for (int col=0; col < (grid_cols); col++){
      Ex_prime[row * grid_cols + col]= 0.5 *(Ex[(row+1) * grid_cols + col] + Ex[row * grid_cols + col]);
      Ey_prime[row * grid_cols + col]= 0.5 * (Ey[row * grid_cols + (col+1)] + Ey[row * grid_cols + col]);
    }
  }
  
  // Output to a file
  ofstream output ("ElectricField.dat");

  // Outputing only one-third of all the array elements to produce a better graphs -- hence row += 3 and col += 3
  for (int row=0; row < (grid_rows); row += 3 ) {
    for (int col=0; col<(grid_cols); col += 3) {

      float x = Ex_prime[row * grid_cols + col];
      float y = Ey_prime[row * grid_cols + col];
      
      //this is used to normalize the vector(This was Marks idea so that the electric field would look better)
      float NormVector = sqrt(x*x + y*y);
      // LAURYNAS: SWAPPED "ROW" AND "COL" IN THE FIRST TWO OUTPUTS -- CURRENT PLOT SEEMS CORRECT. I tend to think that col refers to x-axis and row to y-axis.
      output  << col << " "<< row << " "<< x/NormVector << " " << y/NormVector << endl;

    }
    output << endl;
  }

  output.close();
}
