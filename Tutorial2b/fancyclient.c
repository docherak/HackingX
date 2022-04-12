#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// atoi
#include <errno.h>
#include <arpa/inet.h>	// inet_pton
#include <ctype.h>

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
		printf("Enter the message (type 'exit' to quit): \n");
		
		memset(sent, 0, 1024);
		fgets(sent, sizeof(sent), stdin);
		if (strcmp(sent, "exit\n") == 0) {
			close(sockfd);
			break;
		}

		int lenSent = sendto(sockfd, sent, strlen(sent), 0, (struct sockaddr*)&dest, sizeof(dest));
		
		memset(received, 0, 1024);
		int lenRecv = recvfrom(sockfd, received, 1024, 0, (struct sockaddr*)&dest, &socksz);
		printf("%s", received);
	}

	return 0;
}
