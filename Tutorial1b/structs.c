#include <stdio.h>
#include <string.h>

typedef struct idserver {
	char * id;
	char * region;	// eur asr ame afr
	int latency;	// in usec
	char status[8];	// up down unknown
	int *nthreads;
} idserver;

// typedef struct jmeno_structu jmeno_structu

int main () {
	idserver theServer;
	int number_of_available_threads;
	
	char name[10];
	char geo_region[10];
	strcpy(name,"fujur");
	strcpy(geo_region, "asi");
	
	theServer.id = name;
	theServer.region = geo_region;
	
	theServer.latency = 31400;

	strcpy(theServer.status,"unknown");
	theServer.nthreads = &number_of_available_threads;
	
	// pointers to structs
	idserver *serverPointer;
	serverPointer = &theServer;

	// equivalents
	(*serverPointer).latency = 600;
	serverPointer->latency = 600;

	// test
	printf("%d",theServer.latency);	

	return 0;
}
