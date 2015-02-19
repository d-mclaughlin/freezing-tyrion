# Shell script to run everything in one shot

# If any part of this script fails then this ensures that it exits before running
#   the rest of the program
set -e

if [ "$#" -eq 3 ]; then
  # If you provide 3 arguments then you can change the size of the grid and the name
  #   of the file.
  input_file="$1"
  rows="$2"
  cols="$3"
  
elif [ "$#" -eq 1 ]; then
  # If there's only one arugment then that's the filename, and the 200x200 grid default
  input_file="$1" 
  rows=200
  cols=200

else
  # If there are no arguments then default to a 200x200 grid with systemA.txt
  input_file="misc/systemA.txt"
  rows=200
  cols=200
fi

cd misc
# I can't seem to get this line working with makefile so it's here for now
g++ -c ../src/parser.cpp -std=c++0x -Wall -g
make
make clean
cd ..

echo "Running..."
res/main.exe "$rows" "$cols" "$input_file"
echo "Done!"

# Clean up the directory of all the dat files the program makes
rm -f misc/cpu_start.dat misc/cpu_end.dat misc/time_start.dat misc/time_end.dat misc/memory.dat misc/PID.dat
# Remove the main program as well
rm -f res/main.exe

echo "Plotting..."
gnuplot -e "rows="$rows"; cols="$cols"" misc/potential.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/electric_field.plot

gnuplot -e "rows="$rows"; cols="$cols"" misc/equipotential.plot
echo "Done!"

# Remove those dat files we no longer need
rm -f misc/potential_matrix.dat misc/electric_field.dat misc/equipotential.dat

# These get really annoying when testing a lot of stuff quickly so just uncomment as necessary
gv res/potential.eps
gv res/electric_field.eps
gv res/equipotential.eps
