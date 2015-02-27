CC=g++
objs=parser.o electric_field.o cpu.o error.o print_grid_to_file.o main.o EasyBMP.o
headers=main.h functions.h EasyBMP_BMP.h EasyBMP_DataStructures.h EasyBMP.h EasyBMP_VariousBMPutilities.h
flags=-Wall -g -lm

source_folder=../src
output_folder=../res

$(output_folder)/main.exe: $(objs)
	$(CC) $^ -o $@ $(flags)

%.o: $(source_folder)/%.cpp  
	$(CC) -c $< $(flags)

.PHONY: clean
clean:
	rm -f *.o
