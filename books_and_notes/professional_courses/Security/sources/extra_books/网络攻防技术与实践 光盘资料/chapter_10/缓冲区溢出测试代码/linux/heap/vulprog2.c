#include <stdio.h> 	 
#include <stdlib.h> 	 
#include <unistd.h> 	 
#include <string.h> 	 
#include <dlfcn.h> 	 
#define ERROR -1 	 
#define BUFSIZE 16 	 
int goodfunc(const char *str); 	/* 	funcptr 	starts 	out 	as 	this 	*/ 	 
int main(int argc, char **argv) 
{ 	 
	static char buf[BUFSIZE]; 	 
 	static int (*funcptr)(const char *str); 	 
	if (argc <= 2) 	 
	{ 	 
		fprintf(stderr, "Usage: %s <buffer> <goodfunc's arg>\n", argv[0]); 	 
		exit(ERROR);
	}
	printf("system()'s address = %p\n", &system); 	 
	funcptr = (int(*)(const char *str))goodfunc; 
	printf("before overflow: funcptr points to %p\n", funcptr); 	 
	memset(buf, 0, sizeof(buf)); 
	strncpy(buf, argv[1], strlen(argv[1])); 
	printf("after overflow: funcptr points to %p\n", funcptr); 	 
	(void)(*funcptr)(argv[2]); 
	return 0; 
} 	 

int goodfunc(const char *str) 	 
{ 	 
	printf("\nHi, I'm a good function. I was called through funcptr.\n"); 	 
	printf("I was passed: %s\n", str); 	 
	return 0; 	 
}
