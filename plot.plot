set term postscript
set output "plot.eps"

set pm3d map
set size square
#set palette color
#set palette model RGB
set xrange[0:229]
set yrange[0:229]
set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
     0.75 1 0 0, 1 1 1 1 )

splot "potential.dat" matrix