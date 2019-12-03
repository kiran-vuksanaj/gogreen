ifeq ($DEBUG,true)
	CFLAGS = -g
endif

all: main.o temp.o
	gcc $(CFLAGS) -o shash main.o temp.o
main.o: main.c temp.h
	gcc $(CFLAGS) -c main.c
temp.o: temp.c temp.h
	gcc $(CFLAGS) -c temp.c

clean:
	rm shash
	rm *.o
	rm *~

grind:
	valgrind ./shash --leak-check=yes
