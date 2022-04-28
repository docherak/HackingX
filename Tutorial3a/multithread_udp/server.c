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

#define N_THREADS 1
pthread_mutex_t mutInit = PTHREAD_MUTEX_INITIALIZER;

// Funtion to send message back to the client using multiple arguments in a struct
void *sendMsg(void *ptr);

// Struct used to pass arguments
typedef struct recInfo {
	struct sockaddr_in *dest;
	int sckt;
	char *message;
	int messageLen;
	char *clientIP;
} recInfo;

// Global variable to check the number of threads in use
int currThreadsNo = 0;

// Main program
int main(int argc, char* argv[])
{
	
	// Input check
	if (argc < 2) {
	fprintf(stderr, "Missing argument. Please enter a port number to listen to.\n");
	return 1;
    }

	// Server struct, create and fill the socket
	struct sockaddr_in server;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);	
	if(sockfd == -1) {
		fprintf(stderr, "Failed creating the socket");
		return 1;
	}
	
	socklen_t socksz = sizeof(struct sockaddr);
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));	// parse str as int
	inet_pton(AF_INET, "localhost", &server.sin_addr.s_addr);

	// Initial message	
	printf("<<Server started>>\n");

	// Bind the socket to the port
	if (bind(sockfd, (const struct sockaddr*)&server, sizeof(server))) {
		fprintf(stderr, "Failed binding the socket to the port.\n");
		close(sockfd);
		return 1;
	}

	// Data transfer
	while (1) {

		// https://www.youtube.com/watch?v=ulanSvWou1M - Starter UDP S&C in C++
		// Client identifier		
		struct sockaddr_in *client = malloc(sizeof(struct sockaddr_in));
		socklen_t *clientLen;
		socklen_t temp = (socklen_t) sizeof(struct sockaddr);	
		clientLen = &temp;

		// Receive
		int messageLen = 1024;
		char *message = malloc(messageLen * sizeof(char));
		memset(message, 0, 1024);

		int received = recvfrom(sockfd, message, messageLen, 0, (struct sockaddr*) client, clientLen);		
		//message[received] = '\n';
		
		// Get client info	
		int clientIPLen = 1024;
		char *clientIP = malloc(clientIPLen * sizeof(char));
		memset(clientIP, 0, 1024);
		
		inet_ntop(AF_INET, &client->sin_addr, clientIP, 1024);	

		// Print the received message, including sender's info and current thread no.
		pthread_mutex_lock(&mutInit);		
		puts("--------------------");
		printf("From: %s\n", clientIP);
		printf("Received: %s", message);
		printf("\ntid: %ld\n", (long)pthread_self());
		pthread_mutex_unlock(&mutInit);		
		
		// Either send info back to the client using another thread or deliver server overload message
		if (currThreadsNo < N_THREADS) {
			recInfo ri = {
				.dest = client,
				.sckt = sockfd,
				.message = message,
				.messageLen = received,
				.clientIP = clientIP
				
			};
			currThreadsNo++;
			
			pthread_t thread;
			if (pthread_create(&thread, NULL, sendMsg, &ri)) {
				fprintf(stderr, "Error creating thread\n");
				return 1;
			}
		} else {
			pthread_mutex_lock(&mutInit);
			printf("Skipping message: %s\n", message);	
			pthread_mutex_unlock(&mutInit);
			char *serverOverload = "Server is too busy, unable to process your message\n";
			int sent = sendto(sockfd, serverOverload, strlen(serverOverload), 0, (struct sockaddr*) client, socksz);	
		}
		
	}

	return 0;
}

// Functions
void *sendMsg(void *ptr) {

	// Load info into the local variable	
	recInfo si = *((recInfo *) ptr);

	// Wait to receive a message
	usleep(10000);

	// Sent message	
	int sent = sendto(si.sckt, si.message, si.messageLen, 0, (struct sockaddr*) si.dest, sizeof(struct sockaddr));		
	
	// Print the message, client info and current thread id using mutex
	pthread_mutex_lock(&mutInit);
	puts("--------------------");
	printf("To: %s\n", si.clientIP);
	printf("Sent: %s", si.message);
	printf("\ntid: %ld\n", (long)pthread_self());
	pthread_mutex_unlock(&mutInit);

	// Reduce the no. of threads in use and free message memory
	currThreadsNo--;	
	free(si.message);
	return NULL;
}
