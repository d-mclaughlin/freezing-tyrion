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
gnuplot << EOF
  set term postscript

  ###############################
  #     Potential field plot    #
  ###############################

  set output "potential.eps"

  set pm3d map
  set size square
  set xrange[0:"$rows"]
  # The graph is plotted upside down, so reverse the y-axis
  set yrange[0:"$cols"] reverse

  unset xtics
  unset ytics
  set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
       0.75 1 0 0, 1 1 1 1 )

  splot "potential_matrix.dat" matrix

#  ###############################
#  #     Electric field plot     #
#  ###############################

#  set title "Electric Field of The Numerical Solution"
#  set output "electric_field.eps"
#  set style arrow 1
#  set key off
#  plot "electric_field.dat" using 1:2:3:4 with vectors lt 1

#  ###################################################################################
#  #   The equipotential field plot is large enough to fit in its own file I think.  #
#  #   The only problem is making that work with this.                               #
#  #                                                                                 #
#  #   Equipotentials plot                                                           #
#  ###################################################################################
#  set output "equipotential.eps"

#  set title "Equipotential Lines of the Numerical Solution"

#  # The key set to be outside of the plot; possible to change position and turn it off by typing "off" as an option.
#  # DO WE WANT KEY???
#  ######### 
#  set key outside vertical
#  set key left top

#  # Comment out if surface is needed -- will give a grid of lines
#  unset surface

#  # Specify where to draw a contour:
#  #   base - on the grid base;
#  #   surface - on the surfaces themselves; 
#  #   both - draws on both the base and contour
#  #########
#  set contour surface

#  # Type of approximation:
#  #   Can change bspline to linear or cubicspline; bspline is supposed to be the smoothest approximation
#  ##########
#  set cntrparam bspline

#  # Sets the order of bspline approximation -- higher the order, smoother the approximation.
#  #   Relevant if bspline approximation is used in the first place
#  ##########
#  set cntrparam order 4

#  # Contour levels - set how many equipotential lines are needed.
#  #   Incremental mode starts at the potential of -2.5 and increments in steps of 0.5 for 11 times (in this case).
#  #   Auto mode requires a number of contour lines and then it adjust actual number and equipotentials to output the best graph.
#  # CHOOSE EITHER ONE
#  ####################
#  #set cntrparam levels incremental -2.5,0.2,26
#  set cntrparam level auto 26

#  splot "equipotential.dat" matrix with image notitle, "potential_matrix.dat" with lines lt 1 lw 2 title "Potential"
EOF
echo "Done!"

# These get really annoying when testing a lot of stuff quickly so just uncomment as necessary
#gv potential.eps 
gv electric_field.eps
#gv equipotential.eps
