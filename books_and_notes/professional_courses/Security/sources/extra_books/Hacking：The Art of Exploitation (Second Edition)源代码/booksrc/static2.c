#include <stdio.h>

void function() { // an example function, with it's own context
	int var = 5;
	static int static_var = 5; // static variable initialization

	printf("\t[in function] var  @ %p = %d\n", &var, var);
	printf("\t[in function] static_var @ %p = %d\n", &static_var, static_var);
	var++;          // add one to var
	static_var++;   // add one to static_var
}

int main() { // the main function, with it's own context
	int i;
	static int static_var = 1337; // another static, in a different context

	for(i=0; i < 5; i++) { // loop 5 times
		printf("[in main] static_var @ %p = %d\n", &static_var, static_var);
		function(); // call the function
	}
}
	
