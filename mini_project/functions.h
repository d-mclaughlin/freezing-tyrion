// Function to find the electric field when the potential is already known.
void electric_field(float v[], int grid_rows, int grid_cols, float grid_spacing);

// Function to equate two arrays in order to renew values in an older array
void equate_matrix(float old_v[], float new_v[], int grid_rows, int grid_cols);

// Function to find the largest absolute error between the two elements of the corresponding arrays passed as the arguments. 
float error_check(float old_v[], float new_v[], int grid_rows, int grid_cols, float error_bound);

// Function to print the array to a data file in a form of a matrix
void fprint_matrix(float v[], int grid_rows, int grid_cols);

// Function to print the data required to plot a graph of potential lines corresponding to th potential defined in an array v (argument of the function). GnuPlot requires to get x and y coordinates as well as the value of a potential at that point.
void data_equipotential(float v[], int grid_rows, int grid_cols);
