/***************************************************************************
    A parser for a simple scripting language for inputting
    initial conditions

    David McLaughlin 26/1/15

    There are currently 3 shapes: box, circle, and plate
    
    Each line in the text file to be interpreted should begin with
    the shape type, then the options in the order listed below,
    then a newline.
    The last line in the file should be "done" (without quotes)

    BOX
      box top_row left_col bottom_row right_col
      
      (top_row, left_col) is the top-left coordinate of the box
      (bottom_row, right_col) is the bottom-right coordinate of the box

    CIRCLE
      circle centre_row centre_col radius
      
      (centre_row, centre_col) is the coordinate of the centre of the circle
      radius is the radius of the circle

    PLATE
      plate (h|v) start_row start_col length voltage
      
      the first option is 'h' or 'v', depending on whether the plate
        is orientated horizontally or vertically
      (start_row, start_col) is the top-left coordinate of the plate
      length is the length of the plate
      voltage is the fixed voltage of the plate

    All coordinates are based on the grid, from the top left, 
    so (200, 200) would be the far bottom right of a 200x200 grid.
    (0, 200) is the bottom left. 
    It wouldn't make sense for a 16x16 grid for example. 
    This is why I'm suggesting we change this into relative 
    coordinates. So (1,1), for example, would always be the
    bottom right.
****************************************************************************/

#include <string>
#include <sstream>    // istringstream
#include "main.h"
#include <iostream>

/* TODO(david): We should get this working with relative coordinates so
 *  we don't need to change the text file every time we change the grid
 *  size.
 *
 * ^^^^ DONE BUT BROKEN kinda
 */

/* TODO(david): Error checking? We're the only ones using this so I
 *  don't reckon it'll be worth it.
 */

void Parse(char *filename, float v[], int grid_rows, int grid_cols) {
  std::ifstream inFile(filename);

  std::string line;

  /* Concept for converting from script coordinates (0 to 1) to 
   * grid coordinates (whatever to whatever):
   *
   * Take the number in the script, multiply by the grid_cols or grid_rows
   * as appropriate.
   */

  while(std::getline(inFile, line)) {
    std::string shape_type;
    std::istringstream linestream(line);

    // Get the first word in the line, which is the type of shape
    linestream >> shape_type;

    if (!shape_type.compare("box")) {
      // Process the rest of the line to get the coordinates
      std::string coord;
      float coords[4];

      int i = 0;
      while (linestream.good() && i < 4) {
        linestream >> coord;
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [top row, left col, bottom row, right col]

      // Convert coordinates
      int top_row = floor(coords[0] * grid_rows);
      int bottom_row = floor(coords[2] * grid_rows);
      int left_col = floor(coords[1] * grid_cols);
      int right_col = floor(coords[3] * grid_cols);

      // Set each point inside the box to 0
      for (int row = top_row; row < bottom_row; row++) {
        for (int col = left_col; col < right_col; col++) {
          v[row * grid_cols + col] = 0;
        }
      }

    } else if (!shape_type.compare("circle")) {
      // Process the rest of the line to get the coordinates
      std::string coord;
      float coords[3];

      int i = 0;
      while (linestream.good() && i < 3) {
        linestream >> coord;
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [centre_row, centre_col, radius]

      // Convert coordinates
      int centre_row = floor(coords[0] * grid_rows);
      int centre_col = floor(coords[1] * grid_cols);
      // NOTE(david): This line is the reason we can't use rectangular grids.
      int radius = coords[2] * grid_rows;

      for (int row = (centre_row - radius); row < (centre_row + radius); row++) {
        for (int col = (centre_col - radius); col < (centre_col + radius); col++) {
          if ((row - centre_row) * (row - centre_row) +
              (col - centre_col) * (col - centre_col) <= radius * radius) {
            v[row * grid_cols + col] = 0;
          }
        }
      }

    } else if (!shape_type.compare("plate")) {
      // Process the rest of the line to get the coordinates
      std::string coord;
      int coords[3];
      
      char orientation;
      linestream >> orientation;

      int i = 0;
      while (linestream.good() && i < 3) {
        linestream >> coord;
        coords[i] = std::stoi(coord);
        i++;
      }
      // Coords is now [starting_row, starting_col, length]

      // Convert coordinates
      int starting_row = floor(coords[0] * grid_rows);
      int starting_col = floor(coords[1] * grid_cols);

      float voltage;
      linestream >> voltage;

      if (orientation == 'h') {
        int length = coords[2] * grid_cols;

        for (int col = starting_col; col < (starting_col + length); col++) {
          if (starting_row == grid_rows) {
            v[(starting_row - 1) * grid_cols + col] = voltage;
          } else {
            v[starting_row * grid_cols + col] = voltage;
          }

        }
      } else if (orientation == 'v') {
        int length = coords[2] * grid_rows;

        for (int row = starting_row; row < (starting_row + length); row++) {
          if (starting_col == grid_cols) {
            v[row * grid_cols + (starting_col - 1)] = voltage;
          } else {
            v[row * grid_cols + starting_col] = voltage;
          }
        }
      }

    } else if (!shape_type.compare("#")) {
      // This is just for comments; it's not actually necessary but whatever
      continue;
    }
  }
}
/*
#include <fstream>
int main(void) {
  int grid_rows = 200;
  int grid_cols = 200;
  float v[200 * 200] = {};
  Parse("systemA.txt", v, grid_rows, grid_cols);

  std::ofstream file("potential.dat", std::ofstream::out);
  for (int row = 0; row < grid_rows; row++) {
    for (int col = 0; col < grid_cols; col++) {
      file << v[row * grid_cols + col] << " ";
    }
    file << "\n";
  }
  file.close();
}
*/