#include "main.h"

// Print a matrix into a file.
// Style is 0 if you don't want the coordinates as well, and 1 if you do
void print_grid_to_file(char *filename, Grid *grid, int style) {
  std::ofstream output (filename);
  if (!output.is_open()) {
    std::cout << "Error opening the data file \"" << filename << "\" \n";
    return;
  }
  // Print to a file the values of the grid.
  for (int row = 0; row < grid->rows; row++) {
    for (int col = 0; col < grid->cols; col++) {
      if (style == 0) {
        // Print the values as "value"
        output << grid->get(row, col) << " ";
      } else {
        // Print the values as "x y value"
        output << col << row << grid->get(row,col) << " ";
      }
    }
    output << "\n";
  }

  output.close();
}
