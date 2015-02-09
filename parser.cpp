/***************************************************************************
    A parser for a simple scripting language for inputting
    initial conditions

    David McLaughlin 26/1/15

    There are currently 3 shapes: box, circle, and plate
    
    Each line in the text file to be interpreted should begin with
    the shape type, then the options in the order listed below,
    then a newline.

    BOX
      box left_col top_row right_col bottom_row
      
      (left_col, top_row) is the top-left coordinate of the box
      (right_col, bottom_row) is the bottom-right coordinate of the box

    CIRCLE
      circle centre_col centre_row radius
      
      (centre_col, centre_row) is the coordinate of the centre of the circle
      radius is the radius of the circle

    PLATE
      plate (h|v) start_col start_row length voltage
      
      the first option is 'h' or 'v', depending on whether the plate
        is orientated horizontally or vertically
      (start_col, start_row) is the top-left coordinate of the plate
      length is the length of the plate
      voltage is the fixed voltage of the plate

    All coordinates are based on the grid, from the top left, 
    so (1, 1) would be the far bottom right.
    (0, 1) is the bottom left. 
****************************************************************************/

#include <string>
#include <sstream>    // istringstream
#include "main.h"

/* TODO(david): Error checking? We're the only ones using this so I
 *  don't reckon it'll be worth it.
 */

void parse(char* filename, float v[], int grid_rows, int grid_cols) {
  std::ifstream inFile(filename);

  std::string line;

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
      // Coords is now [left column, top row, right column, bottom row]

      // Convert coordinates
      int top_row = floor(coords[1] * grid_rows);
      int bottom_row = floor(coords[3] * grid_rows);
      int left_col = floor(coords[0] * grid_cols);
      int right_col = floor(coords[2] * grid_cols);

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
      // Coords is now [centre column, centre row, radius]

      // Convert coordinates
      int centre_row = floor(coords[1] * grid_rows);
      int centre_col = floor(coords[0] * grid_cols);
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
      float coords[3];
      
      char orientation;
      linestream >> orientation;

      int i = 0;
      while (linestream.good() && i < 3) {
        linestream >> coord;
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [starting column, starting row, length]

      // Convert coordinates
      int starting_row = floor(coords[1] * grid_rows);
      int starting_col = floor(coords[0] * grid_cols);

      float voltage;
      linestream >> voltage;

      if (orientation == 'h') {
        int length = floor(coords[2] * grid_cols);

        for (int col = starting_col; col < (starting_col + length); col++) {
          if (starting_row == grid_rows) {
            v[(starting_row - 1) * grid_cols + col] = voltage;
          } else {
            v[starting_row * grid_cols + col] = voltage;
          }

        }
      } else if (orientation == 'v') {
        int length = floor(coords[2] * grid_rows);

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