set term postscript
set output "ElectricField.eps"

set title "Electric Field of The Numerical Solution"
set xrange[0:99]
set yrange[0:99]
set key off
set size square

plot "ElectricField.dat" using 1:2:3:4 with vectors lt 1 

