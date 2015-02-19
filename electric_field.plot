set term postscript
set output "electric_field.eps"

set title "Electric Field of The Numerical Solution"

set pm3d map
set size square
set xrange[0:cols]
set yrange[0:rows] reverse
unset xtics
unset ytics

set style arrow 1
set key off
plot "electric_field.dat" using 1:2:3:4 with vectors lt 1
