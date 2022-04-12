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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"url.h"

/**
 * parse a URL and store the information in info.
 * return 0 on success, or an integer on failure.
 *
 * Note that this function will heavily modify the 'url' string
 * by adding end-of-string delimiters '\0' into it, hence chunking it into smaller pieces.
 * In general this would be avoided, but it will be simpler this way.
 */

int parse_url(char* url, url_info *info)
{
	// url format: [http://]<hostname>[:<port>]/<path>
	// e.g. https://www.polytechnique.edu:80/index.php

	char *column_slash_slash, *host_name_path, *protocol;

	column_slash_slash = strstr(url, "://");
	// it searches :// in the string. if exists,
	//return the pointer to the first char of the match string

	if(column_slash_slash != NULL){ //protocol type exists
		*column_slash_slash = '\0'; //end of the protocol type - ask for clarification on this
		host_name_path = column_slash_slash + 3; //jump to the host name
		protocol = url;
	} else {	//no protocol type: using http by default.
		host_name_path = url;
		protocol = "http";
	}

	info->protocol = protocol;

	if(strcmp(info->protocol,"http") != 0){
		return PARSE_URL_PROTOCOL_UNKNOWN;
	}

	
	char *first_slash;
	first_slash = strchr(host_name_path,'/');
	printf("%s\n", first_slash);
	if(first_slash == NULL){
		return PARSE_URL_NO_SLASH;
	} else {
		*first_slash = '\0';
		info->host = host_name_path;
		info->path = first_slash + 1; // without the '/'
	}
	
	char *port = strchr(host_name_path,':');
	if(port == NULL){
	//	info->port = 80;
		info->port = "80";
	} else {
		*port = '\0';
		info->port = port + 1;
		
	//	if(sscanf(port, "%d",&info->port) != 1) {
	//		return PARSE_URL_INVALID_PORT;
		}
		
	// If everything went well, return 0.
	return 0;
}

/**
 * print the url info to std output
 */
void print_url_info(url_info *info){
	printf("The URL contains following information: \n");
	printf("Protocol:\t%s\n", info->protocol);
	printf("Host name:\t%s\n", info->host);
	// printf("Port No.:\t%d\n", info->port);
	printf("Port No.:\t%s\n", info->port);
	printf("Path:\t\t/%s\n", info->path); // "/" is from here
}
