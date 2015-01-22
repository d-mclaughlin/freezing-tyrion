#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {

  const int Nx = 135;
  const int Ny = 230;

  double V[Nx][Ny] = {0};

  double left_plate = 1.0;
  double right_plate = -1.0;

  for (int j=0; j<Ny; j++) {
    V[0][j] = left_plate;
    V[Nx-1][j] = right_plate;
  }

  for (int j=1; j<(Ny-1); j++) {
    for (int i=1; i<(Nx-1); i++) {
      V[i][j] = (1/4.0)*(V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]);
    }
  }

  double s = 1.6;
  double temp, max_iterate = 500, err_bound = pow(10, -6);
  for (int k=1; k<=max_iterate; k++) {
    for (int j=1; j<(Ny-1); j++) {
      for (int i=1; i<(Nx-1); i++) {
	V[i][j] = (1-s)*V[i][j] + (s/4)*(V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]);
      }
    }
  }

  ofstream output ("potential.dat");

  for (int j=Ny-1; j>=0; j--) {
    for (int i=0; i<Nx; i++) {
      output << setw(15) << V[i][j];
    }
    output << endl;
  }

  output.close();

  return 0;
}
