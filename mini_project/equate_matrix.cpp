#include "main.h"

void equate_matrix(float old_v[], float new_v[], int grid_rows, int grid_cols) {
  
  for (int row=0; row<grid_rows; row++) {
    for (int col=0; col<grid_cols; col++) {
      old_v[row * grid_cols + col] = new_v[row * grid_cols + col];
    }
  }
}


//Uncomment the following to check the above function
/*

int main() {
  float v[9];
  int k=1;
  int grid_rows = 3;
  int grid_cols = 3;
  for (int row=0; row < 3; row++) {
    for (int col=0; col < 3; col++) {
      v[row * 3 + col] = k;
      k++;
    }
  }

  float new_v[9] = {0};

  for (int row=0; row<3; row++) {
    cout << v[row * 3 + 0] << " " << v[row * 3 + 1] << " " << v[row * 3 + 2] << '\t' << new_v[row * 3 + 0] << " " << new_v[row * 3 + 1] << " " << new_v[row * 3 + 2] << endl;
  }

  equate_matrix(v, new_v, grid_rows, grid_cols);

  cout << endl;
  for (int row=0; row<3; row++) {
    cout << v[row * 3 + 0] << " " << v[row * 3 + 1] << " " << v[row * 3 + 2] << '\t' << new_v[row * 3 + 0] << " " << new_v[row * 3 + 1] << " " << new_v[row * 3 + 2] << endl;
  }

  return 0;
}
*/
