#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	printf("%s is at %p\n", argv[1], getenv(argv[1]));
}
