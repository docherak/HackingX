#include <stdio.h>
#include <string.h>
#include "fancy-hello-world.h"

int main(void) {
	char name[101], output[201];		// init variables (parameters)

	hello_string(name, output);		// call the function

	return 0;
}

void hello_string(char* name, char* output) {	
	printf("Enter the name:\n");		// pre-input question	
	
	strcpy(name, fgets(name, 101, stdin));	// copy inputted string to "name"
	strcpy(output, "Hello World, hello ");	// copy the string to output
	
	strcat(output, name);			// append name to output

	printf("%s", output);			// print the sentence
}
