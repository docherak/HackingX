/**
 *  Jiazi Yi
 *
 * LIX, Ecole Polytechnique
 * jiazi.yi@polytechnique.edu
 *
 * Updated by Pierre Pfister
 *
 * Cisco Systems
 * ppfister@cisco.com
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "url.h"
#include "wgetX.h"
#include <stddef.h>
#define CHUNKSIZE 2048

int main(int argc, char* argv[]) {

    url_info info;
    const char * file_name = "pageOutput";
    if (argc < 2) {
	fprintf(stderr, "Missing argument. Please enter URL.\n");
	return 1;
    }

    char *url = argv[1];

    // Get optional file name
    if (argc > 2) {
	file_name = argv[2];
    }

    // First parse the URL
    int ret = parse_url(url, &info);
    if (ret) {
	fprintf(stderr, "Could not parse URL '%s': %s\n", url, parse_url_errstr[ret]);
	return 2;
    }

    //If needed for debug
//    print_url_info(&info);
//    printf("%d\n", ret); 
//    write_data("filenam", "hello", strlen("hello"));

    // Download the page
    struct http_reply reply;

    ret = download_page(&info, &reply);
    if (ret) {
	return 3;
    }

    // Now parse the responses
    char *response = read_http_reply(&reply);
    if (response == NULL) {
	fprintf(stderr, "Could not parse http reply\n");
	return 4;
    }

    // Write response to a file
    write_data(file_name, response, reply.reply_buffer + reply.reply_buffer_length - response);

    // Free allocated memory
    free(reply.reply_buffer);

    // Just tell the user where is the file
    fprintf(stderr, "The file is saved in %s.", file_name);
    return 0;
}

int download_page(url_info *info, http_reply *reply) {

    /*
     * To be completed:
     *   You will first need to resolve the hostname into an IP address.
     *
     *   Option 1: Simplistic
     *     Use gethostbyname function.
     *
     *   Option 2: Challenge
     *     Use getaddrinfo and implement a function that works for both IPv4 and IPv6.
     *
     */

    // Used video "Network Socket in C" by Endian Tribe as a guide/template for this first part
    // https://www.youtube.com/watch?v=MOrvead27B4

    int ret = 0;
    int mySocket;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    struct addrinfo *result;
    struct addrinfo *record;
    
    hints.ai_family = AF_UNSPEC; // works both for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(info->host, info->port, &hints, &result);
    if(ret != 0) {
	    fprintf(stderr, "Error in resolving hostname");
	    exit(1);
    }

    for (record = result; record != NULL; record = record->ai_next) {
	    mySocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
	    if (mySocket == -1) continue; // could not create socket
	    if (connect(mySocket, record->ai_addr, record->ai_addrlen) != -1) break;
    }

    if (record == NULL) {
	    fprintf(stderr, "Connection failed");
	    exit(1);
    }
   
    freeaddrinfo(result); 

    /* To be completed:
     *   Next, you will need to send the HTTP request.
     *   Use the http_get_request function given to you below.
     *   It uses malloc to allocate memory, and snprintf to format the request as a string.
     *
     *   Use 'write' function to send the request into the socket.
     *
     *   Note: You do not need to send the end-of-string \0 character.
     *   Note2: It is good practice to test if the function returned an error or not.
     *   Note3: Call the shutdown function with SHUT_WR flag after sending the request
     *          to inform the server you have nothing left to send.
     *   Note4: Free the request buffer returned by http_get_request by calling the 'free' function.
     *
     */
   
    // Simply followed your guide + man pages here

    char* request = http_get_request(info);
    size_t reqLen = strlen(request);
    if (write(mySocket, request, reqLen) == -1) {
	    fprintf(stderr, "Could not write");
	    exit(1);
    }
    shutdown(mySocket, SHUT_WR);
    free(request);

    /*
     * To be completed:
     *   Now you will need to read the response from the server.
     *   The response must be stored in a buffer allocated with malloc, and its address must be save in reply->reply_buffer.
     *   The length of the reply (not the length of the buffer), must be saved in reply->reply_buffer_length.
     *
     *   Important: calling recv only once might only give you a fragment of the response.
     *              in order to support large file transfers, you have to keep calling 'recv' until it returns 0.
     *
     *   Option 1: Simplistic
     *     Only call recv once and give up on receiving large files.
     *     BUT: Your program must still be able to store the beginning of the file and
     *          display an error message stating the response was truncated, if it was.
     *
     *   Option 2: Challenge
     *     Do it the proper way by calling recv multiple times.
     *     Whenever the allocated reply->reply_buffer is not large enough, use realloc to increase its size:
     *        reply->reply_buffer = realloc(reply->reply_buffer, new_size);
     *
     *
     */

    // Using size_t bc of the unknown sizes

    reply->reply_buffer_length = 0;
    reply->reply_buffer = malloc(sizeof(char[CHUNKSIZE]));
    
    size_t bufferLen = CHUNKSIZE;
    size_t pom = 0;
    do {
	    // StackOverflow for the addition in the line below, answe by R Samuel Klatchko
	    // https://stackoverflow.com/questions/2862071/how-large-should-my-recv-buffer-be-when-calling-recv-in-the-socket-library

	    pom = recv(mySocket, reply->reply_buffer + reply->reply_buffer_length, CHUNKSIZE, 0);
	    if (pom == -1) {
		    fprintf(stderr, "Error during recv");
		    exit(1);
	    }

	    reply->reply_buffer_length += pom;

	    // Reallocation
	    if (reply->reply_buffer_length + CHUNKSIZE > bufferLen) {
		    bufferLen += CHUNKSIZE;
		    reply->reply_buffer = realloc(reply->reply_buffer, bufferLen);
	    }

    } while (pom != 0);

    return 0;
}

void write_data(const char *path, const char * data, int len) {
    
    /*
     * To be completed:
     *   Use fopen, fwrite and fclose functions.
     */
    
    FILE *file;
    
    file=fopen(path, "w");
    if(file==NULL) {
	fprintf(stderr, "Could not open file %s\n", path);
    	return; 
    }
    fwrite(data,sizeof(*data),len,file);
    fclose(file);	
}

char* http_get_request(url_info *info) {
    char * request_buffer = (char *) malloc(100 + strlen(info->path) + strlen(info->host));
    snprintf(request_buffer, 1024, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
	    info->path, info->host);
    return request_buffer;
}

char *next_line(char *buff, int len) {
    if (len == 0) {
	return NULL;
    }
    char *last = buff + len - 1;
    while (buff != last) {
	if (*buff == '\r' && *(buff+1) == '\n') {
		return buff;
	}
	buff++;
    }
    return NULL;
}

char *read_http_reply(struct http_reply *reply) {
    
    // Let's first isolate the first line of the reply
    char *status_line = next_line(reply->reply_buffer, reply->reply_buffer_length);
    if (status_line == NULL) {
	fprintf(stderr, "Could not find status\n");
	return NULL;
    }
    *status_line = '\0'; // Make the first line is a null-terminated string

    // Now let's read the status (parsing the first line)
    int status;
    double http_version;
    int rv = sscanf(reply->reply_buffer, "HTTP/%lf %d", &http_version, &status);
    if (rv != 2) {
	fprintf(stderr, "Could not parse http response first line (rv=%d, %s)\n", rv, reply->reply_buffer);
	return NULL;
    }

    if (status != 200) {
	fprintf(stderr, "Server returned status %d (should be 200)\n", status);
	return NULL;
    }

    /*
     * To be completed:
     *   The previous code only detects and parses the first line of the reply.
     *   But servers typically send additional header lines:
     *     Date: Mon, 05 Aug 2019 12:54:36 GMT<CR><LF>
     *     Content-type: text/css<CR><LF>
     *     Content-Length: 684<CR><LF>
     *     Last-Modified: Mon, 03 Jun 2019 22:46:31 GMT<CR><LF>
     *     <CR><LF>
     *
     *   Keep calling next_line until you read an empty line, and return only what remains (without the empty line).
     *
     *   Difficult challenge:
     *     If you feel like having a real challenge, go on and implement HTTP redirect support for your client.
     *
     */

    char *buf = status_line + 2;
    
    int len = reply->reply_buffer_length - (buf - reply->reply_buffer);
    char *pom;
    
    while (buf != NULL) {
        pom = next_line(buf, len);
        if (buf == pom) return buf + 2; // minus the first line
        pom += 2;
        len -= pom - buf;
        buf = pom;
    }

    return buf;
}

