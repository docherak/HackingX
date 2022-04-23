#include <stdio.h>
#include <stdlib.h>	// atoi
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>	// inet_pton
#include <netinet/in.h>

#define NCLIENTS 50

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

	// Receive data
	char message[1024];
	while (1) {
		memset(message, 0, 1024);
		int received = recvfrom(sockfd, (char *)message, 1024, 0, (struct sockaddr*)&from, &socksz);		
//		message[received] = '\n';

		// Send back to the client
		int sent = sendto(sockfd, (char *)message, strlen(message), 0, (struct sockaddr*)&from, socksz);		
		printf("%s", message);
	}

	return 0;
}
