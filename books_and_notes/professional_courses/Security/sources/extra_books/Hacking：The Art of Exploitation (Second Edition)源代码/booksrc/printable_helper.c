#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define CHR "%_01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"

int main(int argc, char* argv[])
{
   unsigned int targ, last, t[4], l[4];
   unsigned int try, single, carry=0;
   int len, a, i, j, k, m, z, flag=0;
   char word[3][4];
   unsigned char mem[70];

   if(argc < 2) {
      printf("Usage: %s <EAX starting value> <EAX end value>\n", argv[0]);
      exit(1);
   }

   srand(time(NULL));
   bzero(mem, 70);
   strcpy(mem, CHR);
   len = strlen(mem);
   strfry(mem); // randomize
   last = strtoul(argv[1], NULL, 0);
   targ = strtoul(argv[2], NULL, 0);

   printf("calculating printable values to subtract from EAX..\n\n");
   t[3] = (targ & 0xff000000)>>24; // spliting by bytes
   t[2] = (targ & 0x00ff0000)>>16;
   t[1] = (targ & 0x0000ff00)>>8;
   t[0] = (targ & 0x000000ff);
   l[3] = (last & 0xff000000)>>24;
   l[2] = (last & 0x00ff0000)>>16;
   l[1] = (last & 0x0000ff00)>>8;
   l[0] = (last & 0x000000ff);

   for(a=1; a < 5; a++) { // value count
      carry = flag = 0;
      for(z=0; z < 4; z++) { // byte count
         for(i=0; i < len; i++) {
            for(j=0; j < len; j++) {
               for(k=0; k < len; k++) {
                  for(m=0; m < len; m++)
                  {
                     if(a < 2) j = len+1;
                     if(a < 3) k = len+1;
                     if(a < 4) m = len+1;
                     try = t[z] + carry+mem[i]+mem[j]+mem[k]+mem[m];
                     single = (try & 0x000000ff);
                     if(single == l[z])
                     {
                        carry = (try & 0x0000ff00)>>8;
                        if(i < len) word[0][z] = mem[i];
                        if(j < len) word[1][z] = mem[j];
                        if(k < len) word[2][z] = mem[k];
                        if(m < len) word[3][z] = mem[m];
                        i = j = k = m = len+2;
                        flag++;
                     }
                  }
               }
            }
         }
      }
      if(flag == 4) { // if all 4 bytes found
         printf("start: 0x%08x\n\n", last);
         for(i=0; i < a; i++)
            printf("     - 0x%08x\n", *((unsigned int *)word[i]));
         printf("-------------------\n");
         printf("end:   0x%08x\n", targ);

         exit(0);
      }
   }
}
