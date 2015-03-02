CC=g++
objs=parser.o electric_field.o cpu.o error.o print_grid_to_file.o main.o print_error_to_file.o numerical_analytic_comparison.o
headers=main.h functions.h
flags=-Wall -g -std=c++0x

source_folder=../src
output_folder=../res

$(output_folder)/main.exe: $(objs)
	@ $(CC) $^ -o $@ $(flags)

%.o: $(source_folder)/%.cpp  
	@ $(CC) -c $< $(flags)

.PHONY: clean
clean:
	@ rm -f *.o
