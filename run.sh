# Shell script to run everything in one shot

# Set up default values for the size of the grid. The grid can
#   be any size, but the text file needs to be changed to match.
if [ "$#" -eq 2 ]
then
  rows="$1"
  cols="$2"
else
  rows=200
  cols=200
fi

# We need C++11 for some string manipulation functions in parser.cpp
g++ -o main.exe main.cpp parser.cpp find_electric_field.cpp -std=c++0x
# Maybe it's time we had a makefile for that, although it still doesn't
#   take very long so the benefit would be purely aesthetic

./main.exe "$rows" "$cols"

#g++ -o parser.exe parser.cpp -std=c++0x
#./parser.exe

gnuplot << EOF
  ###############################
  #     Potential field plot    #
  ###############################

  set term postscript
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

  splot "potential.dat" matrix

  ###############################
  #     Electric field plot     #
  ###############################

  set output "electricfield.eps"
  set style arrow 1
  plot "electricfield.dat" using 1:2:3:4 with vectors lt 1 
EOF

gv potential.eps
#gv electricfield.eps
