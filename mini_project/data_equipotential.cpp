#include "main.h"

void data_equipotential(float v[], int grid_rows, int grid_cols) {
  ofstream output ("equipotential.dat");
  if (!output.is_open()) {
    cout << "Error openning data file \"equipotential.dat\"" << endl;
    return;
  }

  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      output << col << " " << row << " " << v[row * grid_rows + col] << endl;
    }
    output << endl;
  }

  output.close();
}
