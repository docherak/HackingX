all: hello-world

hello-world: hello-world.o
	cc -g -o hello-world hello-world.o

hello-world.o: hello-world.c
	cc -c -Wall -g hello-world.c

clean:
	rm hello-world.o hello-world
