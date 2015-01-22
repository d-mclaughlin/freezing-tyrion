/*****************************************************************************
 * Attempt to write a function in C++ that would define a circle on a pixel
 * grid. Looks good on a large grid
 *
 *
 *
 *
 *****************************************************************************/

#include <iostream>
#include <iomanip> // setw
#include <fstream> // ofstream
#include <cstdlib> // atoi

using namespace std;

int main(int argc, char *argv[]) {

  // Grid size
  const int Nx = 100;
  const int Ny = 100;  


  // Check for command line arguments, if they are missing define variables by default using the ternary operators

  //Radius of a circle
  //NOTE I am using ints for simplicity -- may be extended to doubles
  int r = (argc > 1) ? atoi(argv[1]) : 3;
  //Position of a centre of a circle
  int a = (argc > 2) ? atoi(argv[2]) : Nx/2;
  int b = (argc > 3) ? atoi(argv[3]) : Ny/2;

  // Define and initialize array to be used to draw a circle on
  double V[Nx][Ny] = { 0 };

  // Now, use the equation of a circle to determine if a pixel on a grid falls in the circle or not. Since we know the radius, we can restrict ourselves to the area of a square of size (2r x 2r) around the centre of a circle.

  // The corner positions of the square
  int x_min = a - r, x_max = a + r, y_min = b - r, y_max = b + r;
  // Distance that we will find and the radius of a circle squared
  double distance_sqrd, r_sqrd = r*r;

  for (int j = y_min; j<=y_max; j++) {
    for (int i = x_min; i<=x_max; i++) {
      distance_sqrd = (i - a)*(i - a) + (j - b)*(j - b);
      if (distance_sqrd <= r_sqrd)
	V[i][j] = 10;
      else
	V[i][j] = 0;
    }
  }

  // Write data to a file to plot
  ofstream pixels ("pixels.dat");

  for (int j = 0; j<Ny; j++) {
    for (int i = 0; i<Nx; i++) {
      pixels << V[i][j] << setw(3);
    }
    pixels << endl;
  }

  pixels.close();

  return 0;
}
