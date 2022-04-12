all : server client fancyclient

server : server.o
	cc -g -o server server.o
server.o : server.c
	cc -c -Wall -g server.c 

client : client.o
	cc -g -o client client.o
client.o : client.c
	cc -c -Wall -g client.c 

fancyclient : fancyclient.o
	cc -g -o fancyclient fancyclient.o
fancyclient.o : fancyclient.c
	cc -c -Wall -g fancyclient.c 

clean:
	rm server.o server client client.o fancyclient fancyclient.o
cleanfancy:
	rm fancyclient fancyclient.o
cleanclient:
	rm client client.o
cleanserver:
	rm server server.o
