set pm3d
splot "potentialDiff.dat" matrix with points ps 0
set term x11 1
set pm3d
set zrange [-1:1]
splot "relativeError.dat" matrix with point ps 0