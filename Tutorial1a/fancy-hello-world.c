#include <stdio.h>
#include <string.h>
#include "fancy-hello-world.h"

int main(void) {
	char name[101], output[200];		// init variables (parameters)
	
	/* 
	
	as the output is used in the strcat() as the destination, it needs to
	be long enough, so we could append name to it with strcat()
	
	*/

	hello_string(name, output);		// call the function

	return 0;
}

void hello_string(char* name, char* output) {	
	printf("Enter the name:\n");		// pre-input question	
	
	strcpy(name, fgets(name, 101, stdin));	// copy inputted string to "name"
	strcpy(output, "Hello World, hello ");	// copy the string to output

	strcat(output, name);			// append name to output
	printf("%s\n", output);			// print the sentence + endline	
}
