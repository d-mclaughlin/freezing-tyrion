/**************************************************************************
	Solving Poisson's equation using the finite difference method

	Morag Deans; David McLaughlin; Martynas Skirbutas; Mark Wood
**************************************************************************/

// NOTE(david): I've just been hammering out what needs to happen to fill that one matrix
//	from http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf .
//	There's nothing very exciting in here I'm afraid.
//	And it doesn't even work so there's always that

// WARNING(david): Lots of maths; not for the faint of heart

#include <stdio.h>

int main() {
	// Define a uniform grid on top of problem area
	int grid_spacing = 1;
	int range_x = 4;
	int range_y = 4;

	// NOTE(david): This 'matrix' is actually 1 dimensional;
	// 	Index a point by saying matrix[row * range_x + column]
	int matrix[(range_x * range_y) * (range_x * range_y)];

	// Find which points lie on the edges, and which are Dirichlet boundaries and 
	// which are von Neumann boundaries.
	for (int i=0; i < (range_x * range_y); i++) {
		// If it's on the top or bottom
		if ((i >= 0 && i < range_x) | 
			(i <= (range_x * range_y) && i > ((range_x * range_y) - range_x))) {
			// Then it's a Dirichlet boundary
			// And the matrix has just a 1 on the diagonal
			
			// NOTE(david): There is probably a better way of doing this
			for (int row=0; row < range_x; row++) {
				for (int col=0; col < range_y; col++) {
					if (row == col) {
						matrix[row * range_x + col] = 1;
					}
				}
			}
		}

		// If it's at the left or right edges
		else if ((i % 4 == 0 || (i+1) % 4 == 0) && (i < ((range_x * range_y) - range_x))) {
			// Then it's a von Neumann boundary
			// And the matrix has a 1 on the diagonal, and a -1 at the place corresponding to its
			// internal neighbour

			// Work out where its neighbour is and make the entry -1
			if (i % 4 == 0) {
				matrix[i + 1] = -1;
			} else if ((i+1) % 4 == 0) {
				matrix[i - 1] = -1;
			}

			// Find the diagonal
			for (int row=0; row < range_x; row++) {
				for (int col=0; col < range_y; col++) {
					if (row == col) {
						// The entry on the diagonal
						matrix[row * range_x + col] = 1;
					}
				}
			}
		}

		else {
			// It's on the inside
			// And the matrix has a -4 on the diagonal, and 1s at the places corresponding to its
			// neighbours

			// TODO(david): There is probably a better way of doing this
			for (int row=0; row < range_x; row++) {
				for (int col=0; col < range_y; col++) {
					if (row == col) {
						// The entry on the diagonal
						matrix[row * range_x + col] = -4;

						// The two next to it on the same row
						matrix[row * range_x + col + 1] = 1;
						matrix[row * range_x + col - 1] = 1;
						
						// The two above and below it
						matrix[(row * range_x + col) + range_x] = 1;
						matrix[(row * range_x + col) - range_x] = 1;
					}
				}
			}
		}
	}

	// Fill a vector with information about the boundary conditions
	// ???(david): What information?

	// Invert the the big matrix
	// NOTE(david): We've already done this
	// NOTE(david): What if we start getting 100x100 matrices? This will be very slow. Do we care?

	// Right multiply the inverted matrix by the boundary conditions vector
	// NOTE(david): We've already done this too

	// Then we get a vector of the potentials at each point on the grid

	// Then the electric field is the vector field E = -grad(potential)

	// Ideally we go through each point in the grid, and work out the electric field at that point,
	// magnitude and direction, then output that data into a file that some plotting program can
	// interpret and make a nice graphic with arrows and stuff.

	for (int row = 0; row < (range_x * range_x); row++) {
		for (int col = 0; col < (range_y * range_y); col++) {
			if ((matrix[row * range_x + col] > 1) |
					((matrix[row * range_x + col] < -4))) {
				matrix[row * range_x + col] = 0;
			}
			printf("%d ", matrix[row * range_x + col]);
		}
		printf("\n");
	}

	return 0;
}