/******************************************************************************
	Solving Poisson's equation using the finite difference method

	Morag Deans, David McLaughlin, Laurinas Mince, Martynas Skirbutas, Mark Wood
*******************************************************************************/

// NOTE(david): I've just been hammering out what needs to happen to fill that one matrix
//	from http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf .
//	There's nothing very exciting in here I'm afraid.

/************************************************************************************************************************************************
																REFERENCES
	Finite-differnce method http://www.ieeeaps.org/pdfs/fa_numerical_poisson_nagel.pdf
	Successive Over-Relaxation http://www.maa.org/publications/periodicals/loci/joma/iterative-methods-for-solving-iaxi-ibi-the-sor-method
	
*************************************************************************************************************************************************/

// WARNING(david): Lots of maths; not for the faint of heart

// printf
#include <stdio.h>
// atoi
#include <cstdlib>
#include <vector>
// sqrt, cos
#include <math.h>

void PrintVector(std::vector<float> vector, int size) {
	for (int col = 0; col < size; col++) {
		printf("%f ", vector[col]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	// Define a uniform grid on top of problem area
	int grid_x = atoi(argv[1]);
	int grid_y = atoi(argv[2]);

	// Use these to reference elements in the matrix
	int matrix_x = grid_x * grid_x;
	int matrix_y = grid_y * grid_y;

	// NOTE(david): This 'matrix' is actually 1 dimensional;
	// 	Index a point by saying matrix[row * matrix_x + column]
	//	It might look silly but trust me it makes everything easier
	std::vector<int> matrix (matrix_x * matrix_y, 0);

	// Find which points lie on the edges, and which are Dirichlet boundaries and 
	// which are von Neumann boundaries.

	// NOTE(david): I'm not too sure on the definitions of Dirichlet and von Neumann
	// 	boundaries so tell me if I'm missing something
	for (int i = 0; i < (grid_x * grid_y); i++) {
		// If it's on the top or bottom
		if ((i >= 0 && i < grid_x) | (i >= ((grid_x * grid_y) - grid_x))) {
			// Then it's a Dirichlet boundary
			// And the matrix has just a 1 on the diagonal
			for (int col = 0; col < matrix_x; col++) {
				if (col == i) {
					matrix[i * matrix_x + col] = 1;
				}
			}
		}

		// If it's at the left or right edges
		else if ((i % grid_x == 0) | ((i+1) % grid_x == 0)) {
			// Then it's a von Neumann boundary
			// And the matrix has a 1 on the diagonal, and a -1 at the place corresponding to its
			// internal neighbour
			for (int col = 0; col < matrix_x; col++) {
				if (col == i) {
					matrix[i * matrix_x + col] = 1;

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
			for (int col = 0; col < matrix_y; col++) {
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
	// 'Information' is non-zero potentials or non-zero charge densities
	// So if a point on our grid is in such an area, it has a non-zero value, else it's 0

	// TODO(david): Work out if our grid points lie in these places, and fill b accordingly
	//	we know the center and radius of the circle, and if we know the grid spacing then we
	//	know which points are inside the circle.
	
	// TODO(david): Get this working with a more complicated case, ie fill b with useful stuff
	
	// NOTE(david): This is just a dumb test case, with two parallel plates,
	//	the top one is at +1V and the bottom one is at -1V, and there's nothing inbetween
	int b[] = {1,1,1,1,0,0,0,0,0,0,0,0,-1,-1,-1,-1};
	std::vector<float> boundaries (b, b + matrix_x);

	// Solve Ax = b
	// Find omega
	float t = cos(M_PI / grid_x) + cos(M_PI / grid_y);
	float omega = ((8 - sqrt(64 - (t*t))) / (t*t));

	// Make initial guess; the zero vector
	std::vector<float> x_old (matrix_x, 0);
	std::vector<float> x (matrix_x, 0);

	int still_guessing = true;
	while (still_guessing) {
		for (int i = 0; i < matrix_x; i++) {
			int diagonal = matrix[i * matrix_x + i];
			
			int first_sum = 0;
			for (int j = 0; j < (i-1); j++) {
				first_sum += matrix[i * matrix_x + j] * x[j];
			}
			int second_sum = 0;
			for (int j = i; j < matrix_y; j++) {
				second_sum += matrix[i * matrix_x + j] * x_old[j];
			}

			x[i] = x_old[i] + (omega / diagonal) * 
				(boundaries[i] - first_sum - second_sum);
		}
		
		// TODO(david): Check if the old vector is so close to the new one that there's 
		// no point in continuing.
		x_old = x;
	}

	// x is now a vector of the potentials at each point on the grid
	printf("X:\n");
	PrintVector(x, matrix_x);

	// Then the electric field is the vector field E = -grad(potential)

	// Ideally we go through each point in the grid, and work out the electric field at that point,
	// magnitude and direction, then output that data into a file that some plotting program can
	// interpret and make a nice graphic with arrows and stuff.

	// All this work for some nice formatting...
	for (int row = 0; row < matrix_x; row++) {
		printf("%d:\t", (row+1));
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
