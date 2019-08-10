#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

/* barf a message and exit */
void barf(char *message, char *extra) {
   printf(message, extra);
   exit(1);
}

/* a dictionary attack example program */
int main(int argc, char *argv[]) {
   FILE *wordlist;
   char *hash, word[30], salt[3];
   if(argc < 2) 
      barf("Usage: %s <wordlist file> <password hash>\n", argv[0]);

   strncpy(salt, argv[2], 2); // first 2 bytes of hash are the salt
   salt[2] = '\0';  // terminate string

   printf("Salt value is \'%s\'\n", salt);

   if( (wordlist = fopen(argv[1], "r")) == NULL) // open the wordlist
      barf("Fatal: couldn't open the file \'%s\'.\n", argv[1]);
   
   while(fgets(word, 30, wordlist) != NULL) { // read each word
      word[strlen(word)-1] = '\0'; // remove the '\n' byte at the end
      hash = crypt(word, salt); // hash the word using the salt
      printf("trying word:   %-30s ==> %15s\n", word, hash);
      if(strcmp(hash, argv[2]) == 0) { // if the hash matches
         printf("The hash \"%s\" is from the ", argv[2]);
         printf("plaintext password \"%s\".\n", word);
         fclose(wordlist);
         exit(0);
      }
   }
   printf("Couldn't find the plaintext password in the supplied wordlist.\n");
   fclose(wordlist);
}
