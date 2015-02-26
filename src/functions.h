#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Function prototypes
void parse(char *filename, Grid *is_fixed, Grid *grid);

void electric_field(Grid *grid);

void cpu_calc(void);

void print_grid_to_file(const char *filename, Grid *grid, int style);

int error_check(Grid *old_grid, Grid *new_grid, int error_bound);
                  
void equate_matrix(Grid *old_grid, Grid *new_grid);

#endif
