ifeq ($(DEBUG),true)
	CFLAGS = -g
endif

all: main.o parseargs.o execute.o prompt.o
	gcc $(CFLAGS) -o shash main.o parseargs.o execute.o prompt.o
main.o: main.c parseargs.h execute.h prompt.h
	gcc $(CFLAGS) -c main.c
parseargs.o: parseargs.c parseargs.h
	gcc $(CFLAGS) -c parseargs.c
execute.o:  execute.c execute.h parseargs.h
	gcc $(CFLAGS) -c execute.c
prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c

run:
	./shash

clean:
	rm shash
	rm *.o
	rm *~

grind:
	valgrind ./shash --leak-check=yes
