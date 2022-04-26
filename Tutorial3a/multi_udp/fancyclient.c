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

pthread_mutex_t mutInit = PTHREAD_MUTEX_INITIALIZER;

// Need to use multiple arguments for a function passed through
// pthread_create, using struct as argument, therefore these two functions:
void *sendMsg(void *sptr);
void *recMsg(void *rptr);

// Struct passed through functions above
typedef struct sendInfo {
	struct sockaddr_in dest;
	int sckt;
} sendInfo;

int quit = 0;

// Main program
int main(int argc, char* argv[])
{
	
	// Input check
	if (argc < 3) {
		fprintf(stderr, "Missing argument. Please enter both IP address and a port number.\n");
		return 1;
	}

	// Struct init
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));	// parse str as int
	inet_pton(AF_INET, argv[1], &dest.sin_addr.s_addr);

	// Socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);	
	
	// Fill the struct to be used in the functions
	sendInfo si = {
		.dest = dest,
		.sckt = sockfd,
	};	
	
	pthread_t sendThread;
	if(pthread_create(&sendThread, NULL, sendMsg, &si)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;	
	}	


	pthread_t recThread;
	if(pthread_create(&recThread, NULL, recMsg, &si)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;	
	}	
	
	while (1) {
		if (quit == 1) {
			printf("Quitting...\n");
			break;
		}
	}

	return 0;
}

void *sendMsg(void *sptr) {
	
	sendInfo si = *((sendInfo *) sptr);

	while (1) {

		usleep(100000); // so reply from server can be read
		pthread_mutex_lock(&mutInit);

		// Read input	
		char sent[1024];
		// char *msg = sent;
		puts("--------------------");
		printf("Enter the message (type 'exit' to quit): / %ld \n", (long)pthread_self());
		memset(sent, 0, 1024);
		fgets(sent, sizeof(sent), stdin);
		if (strcmp(sent, "exit\n") == 0) {
			close(si.sckt);
			quit = 1;
			break;
		}

		// Sent message
		int lenSent = sendto(si.sckt, sent, strlen(sent), 0, (struct sockaddr*)&(si.dest), sizeof(struct sockaddr));
		
		pthread_mutex_unlock(&mutInit);	

	}

	return NULL;
}

void *recMsg(void *rptr) {

	sendInfo ri = *((sendInfo *) rptr);
	int sockfd = ri.sckt;	
	//int sockfd = *((int *) rptr);

	socklen_t socksz = sizeof(ri.dest);
	while (1) {
	
		char received[1024];
		memset(received, 0, 1024);

		// Receive message
		int lenRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&(ri.dest), &socksz);

		// Mutex
		pthread_mutex_lock(&mutInit);
		puts("--------------------");
		printf("Server's response: \n");
		printf("%s / %ld\n", received, (long)pthread_self());
		pthread_mutex_unlock(&mutInit);

	}
	
	return NULL;
}
