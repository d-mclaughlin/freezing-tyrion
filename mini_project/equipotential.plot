#############################################################################
# GNUPLOT script to plot the equipotential lines of the numerical solution
# to the problem A.
#
# It is showing equipotential lines with a palette-mapped data of potentials.
#
# Things to improve:
#		>> set x- and y-axis ranges automatically depending on the
#		   grid size produced
#		>> run gnuplot from c++ code
#		>> change palette
#		>> the key of the plot seems to be slightly out of place -- 
#		   adjust location or remove it at all.
#
# FOLLOW THE COMMENTS FOR EXPLANATIONS OR CHANGING THE PLOT
#
#############################################################################

set term postscript
set output "equipotential.eps"

set title "Equipotential Lines of the Numerical Solution"

# Specify the size of x-axis and y-axis
# NOTE (TO DO): use shell script to adjust these values automatically
set xrange[0:99]
set yrange[0:99]

# The key set to be outside of the plot; possible to change position and turn it off by typing "off" as an option.
# DO WE WANT KEY???
######### 
set key outside vertical
set key left top

set pm3d map

# Comment out if surface is needed -- will give a grid of lines
unset surface

set size square

# Palette of colours that are used to draw a surface and indicate different potentials
set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
     0.75 1 0 0, 1 1 1 1 )

# Specify where to draw a contour; base - on the grid base; surface - on the surfaces themselves; both - draws on both the base and contour
#########
set contour surface

# Type of approximation; can change bspline to linear or cubicspline; bspline is supposed to be the smoothest approximation
##########
set cntrparam bspline

# Sets the order of bspline approximation -- higher the order, smoother the approximation; relevant if bspline approximation is used in the first place
##########
set cntrparam order 4

# Contour levels - set how many equipotential lines are needed; incremental mode starts at the potential of -2.5 and increments in steps of 0.5 for 11 times (in this case); auto mode requires to provide a number of contour lines and then it adjust actual number and equipotentials to output the best graph. CHOOSE EITHER ONE
####################
#set cntrparam levels incremental -2.5,0.2,26
set cntrparam level auto 26

splot "matrix_potential.dat" matrix with image notitle, "potential.dat" with lines lt 1 lw 2 title "Potential"