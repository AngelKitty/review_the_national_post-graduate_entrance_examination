#include <unistd.h>

void lowered_priviledge_function(unsigned char *ptr) {
   char buffer[50];

   seteuid(5);  // drop priviledges to games user

   strcpy(buffer, ptr);
}

int main(int argc, char *argv[]) {
   if (argc > 0)
      lowered_priviledge_function(argv[1]); 
}
