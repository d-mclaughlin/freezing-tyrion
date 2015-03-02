/*
Function to take vector containing data of grid absolute_error for each iteration.

Plot output to see erorr against number of iterations.
*/

#include "main.h"

void print_error_to_file(const char *filename, std::vector<double> absError){
  std::ofstream output (filename);
  //Number of elements in vector.
  int vector_size=absError.size();

  for(int i=0;i<vector_size;i++){
    output<<absError[i]<<"\n";
  }

  output.close();
}
