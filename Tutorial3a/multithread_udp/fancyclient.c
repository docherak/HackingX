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

int readAndSend(char *sent, int sockfd, struct sockaddr_in dest);
int requestResponse(char *received, int sockfd, struct sockaddr_in dest, socklen_t *sockz);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
	socklen_t socksz = sizeof(dest);

	// Send data
	char received[1024];
	char sent[1024];

	while (1) {


//		printf("Enter the message (type 'exit' to quit): \n");
//		
//		memset(sent, 0, 1024);
//		fgets(sent, sizeof(sent), stdin);
//		if (strcmp(sent, "exit\n") == 0) {
//			close(sockfd);
//			break;
//		}

//		int lenSent = sendto(sockfd, sent, strlen(sent), 0, (struct sockaddr*)&dest, sizeof(dest));
		if(readAndSend(sent, sockfd, dest)==0) break;
			
//		memset(received, 0, 1024);
//		int lenRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&dest, &socksz);
		requestResponse(received, sockfd, dest, &socksz);
//		printf("Server's response: \n");
//		printf("%s", received);
	}

	return 0;
}

int readAndSend(char *sent, int sockfd, struct sockaddr_in dest)
{

	printf("Enter the message (type 'exit' to quit): \n");
	memset(sent, 0, 1024);
	fgets(sent, sizeof(sent), stdin);
	if (strcmp(sent, "exit\n") == 0) {
		close(sockfd);
		return 0;
	}

	int lenSent = sendto(sockfd, sent, strlen(sent), 0, (struct sockaddr*)&dest, sizeof(dest));

	return 1;
}

int requestResponse(char *received, int sockfd, struct sockaddr_in dest, socklen_t *socksz)
{
	memset(received, 0, 1024);
	int lenRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&dest, socksz);

	printf("Server's response: \n");
	printf("%s", received);
		
	return 1;
}
