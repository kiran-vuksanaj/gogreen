ifeq ($DEBUG,true)
	CFLAGS = -g
endif

all: main.o temp.o parseargs.o execute.o
	gcc $(CFLAGS) -o shash main.o temp.o
main.o: main.c temp.h parseargs.h execute.h
	gcc $(CFLAGS) -c main.c
temp.o: temp.c temp.h
	gcc $(CFLAGS) -c temp.c
parseargs.o: parseargs.c parseargs.h
	gcc $(CFLAGS) -c parseargs.c
execute.o:  execute.c execute.h parseargs.h
	gcc $(CFLAGS) -c execute.c

run:
	./shash

clean:
	rm shash
	rm *.o
	rm *~

grind:
	valgrind ./shash --leak-check=yes
