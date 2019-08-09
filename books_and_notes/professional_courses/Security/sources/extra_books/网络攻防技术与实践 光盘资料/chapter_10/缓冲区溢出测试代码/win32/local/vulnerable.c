#include <stdio.h>
int main()
{
	int i;
	char name[32];
	gets(name);
	for (i=0; i<32 && name[i];i++)
		printf("\\0x%x",name[i]);
}
