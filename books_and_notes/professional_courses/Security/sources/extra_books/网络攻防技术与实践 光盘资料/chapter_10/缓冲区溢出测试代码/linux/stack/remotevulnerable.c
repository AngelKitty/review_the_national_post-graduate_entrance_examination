#include <stdio.h>

int main(int argc,char ** argv)
{
        char buffer[1000];
        printf("I am here%x,buffer%d\n",buffer,strlen(argv[1]));
        strcpy(buffer,argv[1]);

        return 0;
}

