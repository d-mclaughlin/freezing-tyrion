set term postscript
set output "pixels.eps"

set pm3d map
set size square

set palette defined ( 0 0 0 0, 0.5 0 1 0)

splot "pixels.dat" matrix