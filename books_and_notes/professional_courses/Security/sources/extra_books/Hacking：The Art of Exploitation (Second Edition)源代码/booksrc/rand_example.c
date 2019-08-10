#include <stdio.h>
#include <stdlib.h>

int main() {
	int i;
	printf("RAND_MAX is %u\n", RAND_MAX);
	srand(time(0));

	printf("random values from 0 to RAND_MAX\n");
	for(i=0; i < 8; i++)
		printf("%d\n", rand());
	printf("random values from 1 to 20\n");
	for(i=0; i < 8; i++)
		printf("%d\n", (rand()%20)+1);
}
