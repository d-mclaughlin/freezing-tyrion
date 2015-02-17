#include "main.h"

// Set one grid equal to another.

void equate_matrix(Grid *old_grid, Grid *new_grid) {
  // Renewing the values in the previous array with the values in the current array  
  for (int row = 0; row < old_grid->rows; row++) {
    for (int col = 0; col < old_grid->cols; col++) {
      old_grid->set(row, col, new_grid->get(row, col));
    }
  }
}
