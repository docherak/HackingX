#include <stdio.h>

int main () {
	
	int var = 20;	// actual variable declaration
	int *alias;	// defining a pointer variable alias

	alias = &var;	// store address of var in pointer variable alias

	printf("Address of var variable: %p\n", &var);
	
	// address stored in pointer variable
	printf("Address stored in alias variable: %p\n", alias);

	// access the value using the pointer
	printf("Value of *alias (stored in memory cell with address '%p'): %d\n", alias, *alias);

	return 0;

}
