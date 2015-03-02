set term postscript
set output "res/relative_error.eps"

set pm3d map
set size square
set xrange[0:rows]
# The graph is plotted upside down, so reverse the y-axis
set yrange[0:cols] reverse

set zrange [0:1]

unset xtics
unset ytics
set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
     0.75 1 0 0, 1 1 1 1 )

splot "misc/relative_error.dat" matrix
