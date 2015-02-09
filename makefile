CC=g++
objs=parser.o electric_field.o main.o
headers=main.h
flags=-std=c++0x -Wall -g

main.exe: $(objs)
	$(CC) $^ -o $@ $(flags)

%.o: %.c $(headers)
	$(CC) -c $< $(flags)

.PHONY: clean
clean:
	rm -f *.o