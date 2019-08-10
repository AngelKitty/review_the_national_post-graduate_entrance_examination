#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LEN 40

/* barf a message and exit */
void barf(char *message, void *extra) {
   printf(message, extra);
   exit(1);
}

/* pretend this function updates a product description in a database */
void update_product_description(char *id, char *desc)
{
   char product_code[5];
   strcpy(product_code, id);

   // update database
   printf("Updating product #%s with description \'%s\'\n", product_code, desc);
}

int main(int argc, char *argv[], char *envp[])
{
  int i;
  char *id, *desc;

  if(argc < 2)
     barf("Usage: %s <id> <description>\n", argv[0]);
  id = argv[1];   // id - product code to update in DB 
  desc = argv[2]; // desc - item description to update

  if(strlen(id) > MAX_ID_LEN) // id must be less than MAX_ID_LEN bytes
     barf("Fatal: id argument must be less than %u bytes\n", (void *)MAX_ID_LEN);

  for(i=0; i < strlen(desc)-1; i++) { // only allow printable bytes in desc
     if(!(isprint(desc[i]))) 
        barf("Fatal: description argument can only contain printable bytes\n", NULL);
  }

  // clearing out the stack memory (security)
  // clearing all arguments except the first and second
  memset(argv[0], 0, strlen(argv[0]));
  for(i=3; argv[i] != 0; i++)
    memset(argv[i], 0, strlen(argv[i]));
  // clearing all environment variables
  for(i=0; envp[i] != 0; i++)
    memset(envp[i], 0, strlen(envp[i]));

  printf("[DEBUG]: desc argument is at %p\n", desc);
  update_product_description(id, desc); // update database
}
