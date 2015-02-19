CC=g++
objs=parser.o electric_field.o cpu.o error.o print_grid_to_file.o main.o
headers=main.h functions.h
flags=-Wall -g

main.exe: $(objs)
	$(CC) $^ -o $@ $(flags)

%.o: %.cpp $(headers)  
	$(CC) -c $< $(flags)

.PHONY: clean
clean:
	rm -f *.o
