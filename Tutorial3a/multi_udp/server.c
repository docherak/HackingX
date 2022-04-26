#include <stdio.h>
#include <stdlib.h>	// atoi
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>	// inet_pton
#include <netinet/in.h>

#include <pthread.h>
#include <unistd.h>

#define N_THREADS 5
pthread_mutex_t mutInit = PTHREAD_MUTEX_INITIALIZER;

void *recMsg(void *ptr);

typedef struct recInfo {
	struct sockaddr_in *dest;
	int sckt;
} recInfo;

int currThreadsNo = 0;

int main(int argc, char* argv[])
{
	
	// Input check
	if (argc < 2) {
	fprintf(stderr, "Missing argument. Please enter a port number to listen to.\n");
	return 1;
    }

	// Struct init
	struct sockaddr_in from;
	memset(&from, 0, sizeof(from));
	from.sin_family = AF_INET;
	from.sin_port = htons(atoi(argv[1]));	// parse str as int
	inet_pton(AF_INET, "localhost", &from.sin_addr.s_addr);

	// Socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);	
	socklen_t socksz = sizeof(from);

	// Bind the socket to the port
	if (bind(sockfd, (struct sockaddr*)&from, sizeof(from))) {
		fprintf(stderr, "Failed binding the socket to the port.\n");
		return 1;
	}

	// Data transfer
	char message[1024];
	while (1) {
		
		// Receive
		memset(message, 0, 1024);
		int received = recvfrom(sockfd, (char *)message, 1024, 0, (struct sockaddr*)&from, &socksz);		
		//message[received] = '\n';
		
		//pthread_mutex_lock(&mutInit);	
		printf("Received: %s", message);
		//pthread_mutex_unlock(&mutInit);	
		
		// Send back to the client
		int sent = sendto(sockfd, (char *)message, strlen(message), 0, (struct sockaddr*)&from, socksz);		
		printf("Sent: %s", message);
	}

	return 0;
}

void *recMsg(void *ptr) {

	return NULL;
}
