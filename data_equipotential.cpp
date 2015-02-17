#include "main.h"

void data_equipotential(Grid *grid) {
  std::ofstream output ("equipotential.dat");
  if (!output.is_open()) {
    std::cout << "Error opening data file \"equipotential.dat\" \n";
    return;
  }
  // Printing the x and y coordinates as well as the value of a potential at that point to a data file
  for (int row = 0; row < grid->rows; row++) {
    for (int col = 0; col < grid->cols; col++) {
      output << col << " " << row << " " << grid->get(row, col) << " ";
    }
    output << "\n";
  }

  output.close();
}
