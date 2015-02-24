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

void parse(char *filename, Grid *is_fixed, Grid *grid) {
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
        // Take the next coordinate from the line
        linestream >> coord;
        // Convert it to float and put it in the coords array
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [left column, top row, right column, bottom row]

      // Convert coordinates
      int top_row = floor(coords[1] * grid->rows);
      int bottom_row = floor(coords[3] * grid->rows);
      int left_col = floor(coords[0] * grid->cols);
      int right_col = floor(coords[2] * grid->cols);

      // Set each point inside the box to 0
      for (int row = top_row; row < bottom_row; row++) {
        for (int col = left_col; col < right_col; col++) {
          // It's a fixed value
          is_fixed->set(row, col, 1);
          // And that value is 0. We could change this so it's anything
          grid->set(row, col, 0);
        }
      }

    } else if (!shape_type.compare("circle")) {
      // Process the rest of the line to get the coordinates
      std::string coord;
      float coords[3];

      int i = 0;
      while (linestream.good() && i < 3) {
        // Take the next coordinate from the line
        linestream >> coord;
        // Convert it to float and put it in the coords array
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [centre column, centre row, radius]

      // Convert coordinates
      int centre_row = floor(coords[1] * grid->rows);
      int centre_col = floor(coords[0] * grid->cols);
      // NOTE(david): This line is the reason we can't use rectangular grids.
      int radius = coords[2] * grid->rows;

      for (int row = (centre_row - radius); row < (centre_row + radius); row++) {
        for (int col = (centre_col - radius); col < (centre_col + radius); col++) {
          if ((row - centre_row) * (row - centre_row) +
              (col - centre_col) * (col - centre_col) <= radius * radius) {
            
            // It's a fixed value
            is_fixed->set(row, col, 1);
            // And that value is 0. We could change this so it's anything
            grid->set(row, col, 0);
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
        // Take the next coordinate from the line
        linestream >> coord;
        // Convert it to float and put it in the coords array
        coords[i] = std::stof(coord);
        i++;
      }
      // Coords is now [starting column, starting row, length]

      // Convert coordinates
      int starting_row = floor(coords[1] * grid->rows);
      int starting_col = floor(coords[0] * grid->cols);

      float voltage;
      linestream >> voltage;

      if (orientation == 'h') {
        int length = floor(coords[2] * grid->cols);

        for (int col = starting_col; col < (starting_col + length); col++) {
          if (starting_row == grid->rows) {
            // It's a fixed value
            is_fixed->set((starting_row - 1), col, 1);
            // And that value is the voltage
            grid->set((starting_row - 1), col, voltage);
          } else {
            // It's a fixed value
            is_fixed->set(starting_row, col, 1);
            // And that value is the voltage
            grid->set(starting_row, col, voltage);
          }

        }
      } else if (orientation == 'v') {
        int length = floor(coords[2] * grid->rows);

        for (int row = starting_row; row < (starting_row + length); row++) {
          if (starting_col == grid->cols) {
            is_fixed->set(row, (starting_col - 1), 1);
            grid->set(row, (starting_col - 1), voltage);
          } else {
            is_fixed->set(row, starting_col, 1);
            grid->set(row, starting_col, voltage);
          }
        }
      }

    } else if (!shape_type.compare("bitmap")) {
      std::string temporary;
      linestream >> temporary;

      // convert string to const char*
      const char *bmp_file = (char*)temporary.c_str();

      BMP inFile;
      inFile.ReadFromFile("../misc/test.bmp");

      // In this version I am assuming that the user knows the dimensios of the bmp file. It is not impossible -- I am using program which shows the dimensions, i.e. pixels in x and y directions
      //int grid_cols = grid->cols;
      //int grid_rows = grid->rows;
      int grid_cols = 200;
      int grid_rows = 200;

      // Currently there are three colours, I am setting their values here. Could be improved so that the user is able to set these values
      float red_voltage = 100.00f;
      float blue_voltage = -100.00f;
      float green_voltage = 0.00f;

      RGBApixel temp;
      for (int col=0; col < grid_cols; col++) {
	for (int row=0; row < grid_rows; row++) {
	  temp = inFile.GetPixel(col,row);
	  // RED
	  if ((int) temp.Red != 0 && (int) temp.Green == 0 && (int) temp.Blue == 0) {
	    is_fixed -> set(row, col, 1);
	    grid->set(row,col, red_voltage);
	  }
	  // BLUE
	  else if ((int) temp.Red == 0 && (int) temp.Green == 0 && (int) temp.Blue != 0) {
	    is_fixed -> set(row, col, 1);
	    grid -> set(row, col, blue_voltage);
	  }
	  // GREEN
	  else if ((int) temp.Red == 0 && (int) temp.Green != 0 && (int) temp.Blue == 0) {
	    is_fixed -> set(row, col, 1);
	    grid -> set(row, col, green_voltage);
	  }
	  // OTHER COLOURS TREAT AS THE SAME
	  else {
	    grid -> set(row, col, 0);
	  }
	}
      }
    }
    else if (!shape_type.compare("#")) {
      // This is just for comments; it's not actually necessary but whatever
    }
  }
}
