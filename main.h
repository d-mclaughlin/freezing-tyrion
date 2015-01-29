#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

#define array[row, col] array[row * grid_cols + col]

void Parse(char *filename, float v[], int grid_rows, int grid_cols);

#endif