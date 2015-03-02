#include "main.h"

/* Create data for absolute and relative error graphs.
 *
 * Assumes a square matrix.
 */
void numerical_analytic_comparison(Grid *num) {
  int row_size = num->rows;
  int col_size = num->cols;
  std::cout << "Matrix Size: " << row_size << " X " << col_size << "\n";
  // Read in data files and store here.
  // Shouldn't need to actually store these. Just for testing.
  std::vector< std::vector<double> > ana(row_size,std::vector<double>(col_size));
  std::vector< std::vector<double> > dif(row_size,std::vector<double>(col_size));

  // Pick analytical data with suitable matrix size to compare with.
  //  If no suitable file exists, just pick the 50x50 one.
  //  The output will be complete nonsense for this case.
  std::ifstream analytical;

  //Assumes numerical solution is square matrix of length row_size
  switch (row_size) {
    case 50:
      analytical.open("misc/CylinderPotential50.txt");
      break;
    case 100:
      analytical.open("misc/CylinderPotential100.txt");
      break;
    case 101:
      analytical.open("misc/CylinderPotential101.txt");
      break;
    case 500:
      analytical.open("misc/CylinderPotential500.txt");
      break;
    case 501:
      analytical.open("misc/CylinderPotential501.txt");
      break;
    default:
      std::cout << "WARNING: analytical data of size '" << 
        row_size << "' not found. Defaulting to 50 x 50.";
      analytical.open("misc/CylinderPotential50.txt");
      break;
  }

  // Compare how many points form the circle in the numerical and analytical data.
  // Interestingly, numerical seems to give larger area for the same radius.

  // Input analytical result data points.
  for (int row = 0; row < row_size; row++) {
    for (int col = 0; col < col_size; col++) {
      double anaElement;

      analytical >> anaElement;  
      ana[row][col] = anaElement;

      dif[row][col] = fabs(anaElement - num->get(row_size-row-1,col_size-col-1));
    }
  }
  
  // Output is by row. Not column like input files.
  // One for absolute difference, other for relative error.
  std::ofstream difference, relative;
  difference.open("misc/difference.dat");
  relative.open("misc/relative_error.dat");
  
  for (int row = 0; row < row_size; row++) {
    for (int col = 0; col < col_size; col++) {
      // Writing abs difference to data file. Calculation done in previous section.
      difference << dif[row][col] << " ";
      
      // If analytical is zero at this point, set the result to zero.
      // Is to avoid dividing by zero. Not sure if this is best way of handling this.
      if (ana[row][col] == 0) {
	      relative << 0 << " ";
      } else {
       	relative << (fabs((dif[row][col]) / ana[row][col])) << " ";
      }
    }
    relative << "\n";
    difference << "\n";
  }
}
