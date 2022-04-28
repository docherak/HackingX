all : server fancyclient 

server : server.o
	cc -g -o server server.o
server.o : server.c
	cc -c -Wall -g server.c 

fancyclient : fancyclient.o
	cc -g -o fancyclient fancyclient.o
fancyclient.o : fancyclient.c
	cc -c -Wall -g fancyclient.c 

clean:
	rm server.o server fancyclient fancyclient.o
cleanfancy:
	rm fancyclient fancyclient.o
cleanserver:
	rm server server.o
