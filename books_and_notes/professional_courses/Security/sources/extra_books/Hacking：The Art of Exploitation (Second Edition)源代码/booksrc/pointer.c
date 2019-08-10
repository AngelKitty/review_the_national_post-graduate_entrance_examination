#include <stdio.h>
#include <string.h>

int main() {
   char str_a[20];  // a 20 element character array
   char *pointer;   // a pointer, meant for a character array
   char *pointer2;  // and yet another one

   strcpy(str_a, "Hello World\n");
   pointer = str_a; // set the first pointer to the start of the array
   printf(pointer);

   pointer2 = pointer + 2; // set the second one 2 bytes further in
   printf(pointer2);       // print it
   strcpy(pointer2, "y you guys!\n"); // copy into that spot
   printf(pointer);        // print again
}
