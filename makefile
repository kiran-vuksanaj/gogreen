ifeq ($(DEBUG),true)
	CFLAGS = -g
endif

all: main.o parseargs.o execute.o prompt.o redirect.o stry_util.o entry_exit.o
	gcc $(CFLAGS) -o shash main.o parseargs.o execute.o prompt.o redirect.o stry_util.o entry_exit.o
main.o: main.c parseargs.h execute.h prompt.h entry_exit.h
	gcc $(CFLAGS) -c main.c
parseargs.o: parseargs.c parseargs.h
	gcc $(CFLAGS) -c parseargs.c
execute.o:  execute.c execute.h parseargs.h redirect.h stry_util.h
	gcc $(CFLAGS) -c execute.c
prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c
redirect.o: redirect.c redirect.h
	gcc $(CFLAGS) -c redirect.c
stry_util.o: stry_util.c stry_util.h
	gcc $(CFLAGS) -c stry_util.c
entry_exit.o: entry_exit.c entry_exit.h
	gcc $(CFLAGS) -c entry_exit.c

run:
	./shash

clean:
	rm shash
	rm *.o
	rm *~
	rm ./#*

grind:
	valgrind ./shash --leak-check=yes
