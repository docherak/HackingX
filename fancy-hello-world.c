#include <stdio.h>
#include <string.h>
#include "fancy-hello-world.h"

int main(void) {
	/* VERSION1
	char output[] = "Hello World, hello ";

	char name[64];
	printf("Enter the name:\n");
	fgets(name, 64, stdin);

	hello_string(name, output);			

	return 0;
	*/

	// VERSION2
	char output[100], name[100];		// init variables (parameters)

	hello_string(name, output);		// call the function

	return 0;
}

void hello_string(char* name, char* output) {	
	/* VERSION1
	strcat(output, name);
	printf("%s", output);		
	*/

	// VERSION2
	printf("Enter the name:\n");		// pre-input question	
	
	strcpy(name, fgets(name, 100, stdin));	// copy inputted string to "name"
	strcpy(output, "Hello World, hello ");	// copy the string to output
	
	strcat(output, name);			// append name to output

	printf("%s", output);			// print the sentence
}
