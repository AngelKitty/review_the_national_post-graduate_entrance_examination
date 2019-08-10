#include <stdio.h>
#include <fcntl.h>

void display_flags(char *, unsigned int);
void binary_print(unsigned int);

int main(int argc, char *argv[]) {
   display_flags("O_RDONLY\t\t", O_RDONLY);
   display_flags("O_WRONLY\t\t", O_WRONLY);
   display_flags("O_RDWR\t\t\t", O_RDWR);
   printf("\n");
   display_flags("O_APPEND\t\t", O_APPEND);
   display_flags("O_TRUNC\t\t\t", O_TRUNC);
   display_flags("O_CREAT\t\t\t", O_CREAT);
   printf("\n");
   display_flags("O_WRONLY|O_APPEND|O_CREAT", O_WRONLY|O_APPEND|O_CREAT);
}

void display_flags(char *label, unsigned int value) {
   printf("%s\t: %d\t:", label, value);
   binary_print(value);
   printf("\n");
}

void binary_print(unsigned int value) {
   unsigned int mask = 0xff000000; // start with a mask for the highest byte
   unsigned int shift = 256*256*256; // start with a shift for the highest byte
   unsigned int byte, byte_iterator, bit_iterator;

   for(byte_iterator=0; byte_iterator < 4; byte_iterator++) {
      byte = (value & mask) / shift; // isolate each byte
      printf(" ");
      for(bit_iterator=0; bit_iterator < 8; bit_iterator++) { // print the byte's bits
         if(byte & 0x80) // if the highest bit in the byte isn't 0
            printf("1");       // print a 1
         else
            printf("0");       // otherwise print a 0
         byte *= 2;         // move all the bits to the left by 1
      }
      mask /= 256;       // move the bits in mask right by 8
      shift /= 256;      // move the bits in shift right by 8
   }
}
