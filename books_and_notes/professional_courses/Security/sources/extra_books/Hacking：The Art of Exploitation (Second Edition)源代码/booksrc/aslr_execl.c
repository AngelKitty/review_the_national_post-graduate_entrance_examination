#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   int stack_var;

   // print an address from the current stack frame
   printf("stack_var is at %p\n", &stack_var); 

   // start aslr_demo, to see how it's stack is arranged
   execl("./aslr_demo", "aslr_demo", NULL);
}

