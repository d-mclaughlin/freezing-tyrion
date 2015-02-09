CC=g++
objs=electric_field.o main.o CPU.o
headers=main.h functions.h
flags=-g

main.exe: $(objs)
	$(CC) $^ -o $@ $(flags)

%.o: %.c $(headers)
	$(CC) -c $<

.PHONY: clean
clean:
	rm -f *.o
