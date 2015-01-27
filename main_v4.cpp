#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {

  // Define a grid.
  // NOTE: The code could be extended by making a varying grid.
  const int grid_rows = 230;
  const int grid_cols = 230;
  // NOTE(Laurynas): Do we want this to be the case all the time?
  const int grid_spacing = 1;

  // Initialise the grid to 0
  double v[grid_rows][grid_cols] = {0};

  // Boundary conditions
  double top_plate = 100.0;
  double bottom_plate = -100.0;

  for (int col=0; col < grid_cols; col++) {
    // Initial conditions for a capacitor
    // NOTE(Laurynas): I did not initialize a circle at this point since all the elements, except from the boundaries, are 0
    v[0][col] = top_plate;
    v[grid_rows - 1][col] = bottom_plate;
  }

  // Find the mid-point in the grid
  double a = grid_cols/2, b = grid_rows/2;
  // The radius of a circle
  // NOTE(Laurynas): We will probably have to generalise the code to accept some other geometrical figures as well, and will need more user defined parameteres of those figures
  double r = 40;
  // Predefining variables referring to the distance from the centre of a circle squared and the radius squared to find out if an element is within a circle or outside
  double distance_sqrd, r_sqrd = r*r;
  for (int row=1; row < (grid_rows-1); row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
      if (distance_sqrd <= r_sqrd)
	v[row][col] = 0;
      else
	v[row][col] = (1/4.0)*(v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
    }
  }

  // Predifining the relaxation factor s
  // NOTE(Laurynas): there is an equation to calculate this factor for different grids, however it is not a general formula -- it is not valid for all the grids. In general this factor is found by trial and error, and it is between 0 and 2. DO WE WANT TO USE AN EQUATION, OR KEEP THE SAME GRID?

  double s = 1.9729;
  double max_iterate = 50, err_bound = pow(10, -6);

  // Improving the accuracy of the solution by applying the successive over/under relaxation method
  // NOTE(Laurynas): Need to extend to incorporate error bound
  for (int k=1; k <= max_iterate; k++) {
    for (int row=1; row < (grid_rows-1); row++) {
      for (int col=1; col < (grid_cols-1); col++) {
	// from the equation of a circle
	distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
	// checking if a grid element is inside a circle
	if (distance_sqrd <= r_sqrd)
	  v[row][col] = 0;
	else
	  v[row][col] = (1-s)*v[row][col] + (s/4.0)*(v[row-1][col] + v[row+1][col] + v[row][col-1] + v[row][col+1]);
      }
    }
  }

  /*Instructions to plot results:
Run code to generate electric.dat
Launch Gnuplot
Enter: plot 'electric.dat' using 1:2:3:4 with vectors head filled lt 2

Individual vectors hard to see at default zoom level for numbers of samples.
   */


  //Raw directional derivatives for each grid point.
  double eX[grid_rows-1][grid_cols-1] = {0};
  double eY[grid_rows-1][grid_cols-1] = {0};

  //X derivatives
  for(int i=0;grid_rows-1>i;i++){
    for(int j=0;grid_cols-1>j;j++){
      eX[i][j]=(v[i][j]-v[i][j+1]);
      //std::cout<<"First: "<<v[i][j]<<" Second: "<<v[i][j+1]<<" Difference: "<<v[i][j]-v[i][j+1]<<std::endl;
    }
  }

  //Y derivatives
  for(int i=0;grid_rows-1>i;i++){
    for(int j=0;grid_cols-1>j;j++){
      eY[i][j]=(v[i][j]-v[i+1][j]);
    }
  }

  //Average of x and y derivative values for square of four points.
  double eAverageX[grid_rows-2][grid_cols-2] = {0};
  double eAverageY[grid_rows-2][grid_cols-2] = {0};

  //X average
  for(int i=0;grid_rows-2>i;i++){
    for(int j=0;grid_cols-2>j;j++){
      eAverageX[i][j]=(eX[i][j]+eX[i+1][j])/2;
    }
  }

  //Y average
  for(int i=0;grid_rows-2>i;i++){
    for(int j=0;grid_cols-2>j;j++){
      eAverageY[i][j]=(eY[i][j]+eY[i][j+1])/2;
    }
  }
  
  
   for(int row=0;row<grid_rows-1;row++){
      for(int col=0;col<grid_cols-1;col++){
	std::cout<<eX[row][col]<<"  ";}
      std::cout<<"\n";
      }

  // file output to record the solution and plot it later
  ofstream output ("electric.dat");

  //y and x pos give origin of current vector.
  int yPos=0;
  for (int row=0; row<grid_rows-2; row++) {
    int xPos=0;
    yPos++;
    for (int col=0; col<grid_cols-2; col++) {
      //Extract Ex and Ey values from matrices.
      double x=eAverageX[row][col];
      double y=eAverageY[row][col];
      //Normalising the output vector.
      double vectorLength=sqrt(x*x + y*y);
      xPos++;
      output << xPos <<" "<< yPos <<" "<<x/vectorLength<<" "<<y/vectorLength<< endl;
    }
  }

  output.close();


  /*
  for (int row=grid_rows-1; row >= 0; row--) {
    for (int col=0; col<grid_cols; col++) {
      output << setw(15) << v[row][col];
    }
    output << endl;
  }

  output.close();*/

  return 0;
}
