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

make

echo "Running..."
./main.exe "$rows" "$cols"
echo "Done!"

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

  splot "potential.dat" matrix

  ###############################
  #     Electric field plot     #
  ###############################

  set title "Electric Field of The Numerical Solution"
  set output "electric_field.eps"
  set style arrow 1
  set key off
  plot "electric_field.dat" using 1:2:3:4 with vectors lt 1

  ###################################################################################
  #   The equipotential field plot is large enough to fit in its own file I think.  #
  #   The only problem is making that work with this.                               #
  ###################################################################################
EOF
echo "Done!"

gv potential.eps
gv electric_field.eps
