#include <stdio.h>
#include <string.h>
char largebuff[] =
"1234512345123451234512345===ABCD";
int main (void)
{
    char smallbuff[16];
    strcpy (smallbuff, largebuff);
}
