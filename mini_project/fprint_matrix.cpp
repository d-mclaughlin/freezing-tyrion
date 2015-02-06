#include "main.h"

void fprint_matrix(float v[], int grid_rows, int grid_cols) {
  ofstream output ("potential_matrix.dat");
  if (!output.is_open()) {
    cout << "Error openning the data file \"potential_matrix.dat\"" << endl;
    return;
  }

  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      output << v[row * grid_cols + col] << " ";
    }
    output << endl;
  }

  output.close();
}
