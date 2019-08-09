#include <stdio.h>
int main ( int argc, char * argv[] )
{
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL );
}
