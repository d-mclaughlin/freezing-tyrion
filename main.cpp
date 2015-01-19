/**************************************************************************
	Solving Poisson's equation using the finite difference method

	Morag Deans; David McLaughlin; Martynas Skirbutas; Mark Wood
**************************************************************************/

// NOTE(david): I've just been hammering out what needs to happen to fill that one matrix
//	from http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf .
//	There's nothing very exciting in here I'm afraid.

// TODO(david): This only works with a simple two plates with nothing in between.
//	So we need to get this working with a more arbitrary case. Case A will be a good
//	place to start, since it's so simple.

// WARNING(david): Lots of maths; not for the faint of heart

#include <stdio.h>

// Define a uniform grid on top of problem area
static int grid_x = 4;
static int grid_y = 4;

// Use these to reference elements in the matrix
int matrix_x = grid_x * grid_x;
int matrix_y = grid_y * grid_y;

int main() {
	// NOTE(david): This 'matrix' is actually 1 dimensional;
	// 	Index a point by saying matrix[row * matrix_x + column]
	//	It might look silly but trust me it makes everything easier
	int matrix[matrix_x * matrix_y];

	// Initialise every element to 0
	for (int row=0; row < matrix_x; row++) { 
		for (int col=0; col < matrix_y; col++) {
			matrix[row * matrix_x + col] = 0;
		}
	}

	// Find which points lie on the edges, and which are Dirichlet boundaries and 
	// which are von Neumann boundaries.
	for (int i=0; i < (grid_x * grid_y); i++) {
		// If it's on the top or bottom
		if ((i >= 0 && i < grid_x) | (i >= ((grid_x * grid_y) - grid_x))) {
			// Then it's a Dirichlet boundary
			// And the matrix has just a 1 on the diagonal
			for (int col=0; col < matrix_x; col++) {
				if (col == i) {
					matrix[i * matrix_x + col] = 1;
				}
			}
		}

		// If it's at the left or right edges
		// doesn't catch 4 and 11
		else if ((i % grid_x == 0) | ((i+1) % grid_x == 0)) {
			// Then it's a von Neumann boundary
			// And the matrix has a 1 on the diagonal, and a -1 at the place corresponding to its
			// internal neighbour

			for (int col=0; col < matrix_x; col++) {
				if (col == i) {
					matrix[i * matrix_x + col] = 1;

					// Work out where its neighbour is and make the entry -1
					if (i % grid_x == 0) {
						matrix[i * matrix_x + col + 1] = -1;
					} else if ((i+1) % grid_x == 0) {
						matrix[i * matrix_x + col - 1] = -1;
					}
				}
			}
		}

		else {
			// It's on the inside
			// And the matrix has a -4 on the diagonal, and 1s at the places corresponding to its
			// neighbours

			for (int col=0; col < matrix_y; col++) {
				if (col == i) {
					// The entry on the diagonal
					matrix[i * matrix_x + col] = -4;

					// The two next to it on the same row
					matrix[(i * matrix_x + col) + 1] = 1;
					matrix[(i * matrix_x + col) - 1] = 1;
					
					// The two above and below it
					matrix[(i * matrix_x + col) + grid_x] = 1;
					matrix[(i * matrix_x + col) - grid_x] = 1;
				}
			}
		}
	}

	// Fill a vector with information about the boundary conditions
	// ???(david): What information?

	// Invert the the big matrix
	// NOTE(david): We've already done this
	// NOTE(david): What if we start getting 100x100 matrices? This will be very slow. Do we care?
	// yes: there's another method in the link at the top

	// Right multiply the inverted matrix by the boundary conditions vector
	// NOTE(david): We've already done this too

	// Then we get a vector of the potentials at each point on the grid

	// Then the electric field is the vector field E = -grad(potential)

	// Ideally we go through each point in the grid, and work out the electric field at that point,
	// magnitude and direction, then output that data into a file that some plotting program can
	// interpret and make a nice graphic with arrows and stuff.

	// All this work for some nice formatting...
	for (int row = 0; row < matrix_x; row++) {
		for (int col = 0; col < matrix_y; col++) {
			if (matrix[row * matrix_x + col] >= 0) {
				printf(" %d", matrix[row * matrix_x + col]);
			} else {
				printf("%d", matrix[row * matrix_x + col]);
			}
		}
		printf("\n");
	}

	return 0;
}