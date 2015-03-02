#####################################################################
#       Last revised: David McLaughlin 19/2/15                      #
#                                                                   #
#   A shell script to compile, run, plot, and display the data for  #
# the group project in P3M Theoretical Physics                      #
#                                                                   #
#   Inputs are a filename for initial conditions, a number of rows, #
# and a number of columns for the grid.                             #
#####################################################################

# If any part of this script fails then this ensures that it exits before running
#   the rest of the program
set -e


if [ "$#" -eq 1 ]; then
  # If there's only one arugment then that's the filename, and the 200x200 grid default
  input_file="$1" 
  rows=200
  cols=200
  
elif [ "$#" -eq 2 ]; then
  # If you give 2 arguments then they're the grid sizes and the default
  # file (system A) is chosen
  input_file="misc/systemA.txt"
  rows="$1"
  cols="$2"
  
elif [ "$#" -eq 3 ]; then
  # If you provide 3 arguments then you can change the size of the grid and the name
  #   of the file.
  input_file="$1"
  rows="$2"
  cols="$3"

else
  # If there are no arguments then default to a 200x200 grid with systemA.txt
  input_file="misc/systemA.txt"
  rows=200
  cols=200
fi

echo -n "Building..."
cd misc
# I can't seem to get this line working with makefile so it's here for now
# The 'proper' way to fix it would be to avoid using string streams in parser
#   and using fscanf() instead. But I really can't be bothered doing that ever
g++ -c ../src/parser.cpp -std=c++0x -Wall -g
make
make clean
cd ..
echo "Done!"

echo "Running..."
res/main.exe "$rows" "$cols" "$input_file"
echo "Done!"

# Clean up the directory of all the dat files the program makes
rm -f misc/cpu_start.dat misc/cpu_end.dat misc/time_start.dat misc/time_end.dat misc/memory.dat

echo -n "Plotting..."
gnuplot -e "rows="$rows"; cols="$cols"" misc/potential.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/electric_field.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/equipotential.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/difference.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/relative_error.plot

echo "Done!"

# Remove those dat files we no longer need
rm -f misc/potential_matrix.dat misc/electric_field.dat misc/equipotential.dat #misc/difference.dat misc/error_convergence.dat misc/relative_error.dat

# These get really annoying when testing a lot of stuff quickly so just uncomment as necessary
#evince -w res/potential.eps
#evince -w res/electric_field.eps
#evince -w res/equipotential.eps
evince -w res/difference.eps
evince -w res/relative_error.eps
