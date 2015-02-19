set term wxt 0
set pm3d
splot "NumericalPotential.dat" matrix with points ps 0
set term wxt 1
set pm3d
splot "CylinderPotential.dat" matrix with point ps 0
