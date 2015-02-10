#include "main.h"

void equate_matrix(float old_v[], float new_v[], int grid_rows, int grid_cols) {

  // Renewing the values in the previous array with the values in the current array  
  for (int row=0; row<grid_rows; row++) {
    for (int col=0; col<grid_cols; col++) {
      old_v[row * grid_cols + col] = new_v[row * grid_cols + col];
    }
  }
}
