CC=g++
objs=cpu.o print_grid_to_file.o electric_field.o error.o parser.o main.o
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
