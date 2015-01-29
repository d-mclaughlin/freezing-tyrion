# Shell script to run everything in one shot

if [ "$#" -eq 2 ]
then
  rows="$1"
  cols="$2"
else
  rows=230
  cols=230
fi

# We need C++11 for some string manipulation functions in parser.cpp
g++ -o main_1D.exe main_1D.cpp parser.cpp find_electric_field.cpp -std=c++0x
# Maybe it's time we had a makefile for that, although it still doesn't
#   take very long so the benefit would be purely aesthetic

./main_1D.exe "$rows" "$cols"

gnuplot << EOF
  set term postscript
  set output "plot.eps"

  set pm3d map
  set size square
  #set palette color
  #set palette model RGB
  set xrange[0:"$rows"]

  # The graph is plotted upside down, so reverse the y-axis
  set yrange[0:"$cols"] reverse

  unset xtics
  unset ytics
  set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
       0.75 1 0 0, 1 1 1 1 )

  splot "potential.dat" matrix
EOF

gv plot.eps