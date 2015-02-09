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
echo "Done!"

gv potential.eps
#gv electricfield.eps
