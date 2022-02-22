all: adresses pointers structs

adresses: adresses.o
	cc -g -o adresses adresses.o

adresses.o: adresses.c
	cc -c -Wall -g adresses.c

pointers: pointers.o
	cc -g -o pointers pointers.o

pointers.o: pointers.c
	cc -c -Wall -g pointers.c

structs: structs.o
	cc -g -o structs structs.o

structs.o: structs.c
	cc -c -Wall -g structs.c

clean:
	rm adresses.o adresses pointers.o pointers structs.o structs
