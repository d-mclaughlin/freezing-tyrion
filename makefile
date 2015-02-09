CC=g++
objs=parser.o electric_field.o main.o
headers=main.h
OFLAGS="-std=c++0x -Wall"
EXEFLAGS=-g -Wall

main.exe: $(objs)
	$(CC) $^ -o $@ $(EXEFLAGS)

%.o: %.c $(headers)
	$(CC) -c $< $(OFLAGS)

.PHONY: clean
clean:
	rm -f *.o