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

// Struct used to pass arguments through functions above
typedef struct sendInfo {
	struct sockaddr_in dest;
	int sckt;
} sendInfo;

// Global variable for 'exit' option
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

	// Thread for sending	
	pthread_t sendThread;
	if(pthread_create(&sendThread, NULL, sendMsg, &si)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;	
	}	

	// Thread for receiving
	pthread_t recThread;
	if(pthread_create(&recThread, NULL, recMsg, &si)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;	
	}	
	
	// Let the main thread run	
	while (1) {
		if (quit == 1) {
			printf("Quitting...\n");
			break;
		}
	}

	return 0;
}

// Functions
void *sendMsg(void *sptr) {
	
	// Load data into local variable
	sendInfo si = *((sendInfo *)sptr);
	
	// Helper variable for input prompt options
	int messNo = 1;
	
	// Basically Tutorial2b code + mutex:
	while (1) {

		pthread_mutex_lock(&mutInit);

		// Read input	
		char sent[1024];
		puts("--------------------");
		
		// Different input prompt for the first message and for the rest
		(messNo == 1) ? printf("Enter your first message (type 'exit' to quit):\n") : printf("Enter next message:\n");
		memset(sent, 0, 1024);
		fgets(sent, sizeof(sent), stdin);
		messNo++;

		// Close the socket if user types 'exit', stop the main thread while loop using global variable and return 0
		if (strcmp(sent, "exit\n") == 0) {
			close(si.sckt);
			quit = 1;
			break;
		}
		
		// Debug - print thread id no.
		printf("\ntid: %ld\n", (long)pthread_self());

		// Sent message
		int lenSent = sendto(si.sckt, sent, strlen(sent), 0, (struct sockaddr*)&(si.dest), sizeof(struct sockaddr));
		
		pthread_mutex_unlock(&mutInit);	
		
		// So the reply from the server can be optionally read before typing prompt
		usleep(5000000);

	}

	return NULL;
}

void *recMsg(void *rptr) {

	// Load data, create helper variable
	sendInfo ri = *((sendInfo *) rptr);
	int sockfd = ri.sckt;	
	socklen_t socksz = sizeof(ri.dest);

	// Basically Tutorial2b code + mutex:
	while (1) {
	
		char received[1024];
		memset(received, 0, 1024);

		// Receive message
		int lenRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&(ri.dest), &socksz);

		// Print the server's response using mutex
		pthread_mutex_lock(&mutInit);
		puts("--------------------");
		printf("Server's response: \n");
		printf("%s\ntid: %ld\n", received, (long)pthread_self());
		printf("Wait for the prompt...\n");
		pthread_mutex_unlock(&mutInit);

	}
	
	return NULL;
}
