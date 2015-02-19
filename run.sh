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
  input_file="systemA.txt"
  rows=200
  cols=200
fi

# I can't seem to get this line working with makefile so it's here for now
g++ -c parser.cpp -std=c++0x -Wall -g
make
make clean

echo "Running..."
./main.exe "$rows" "$cols" "$input_file"
echo "Done!"

# Clean up the directory
rm -f cpu_start.dat cpu_end.dat time_start.dat time_end.dat memory.dat PID.dat
rm -f main.exe

echo "Plotting..."
gnuplot -e "rows="$rows"; cols="$cols"" potential.plot

gnuplot -e "rows="$rows"; cols="$cols"" electric_field.plot

gnuplot -e "rows="$rows"; cols="$cols"" equipotential.plot
echo "Done!"

# These get really annoying when testing a lot of stuff quickly so just uncomment as necessary
gv potential.eps 
gv electric_field.eps
gv equipotential.eps
