#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
   int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
   struct sockaddr_in host_addr, client_addr;   // my address information
   socklen_t sin_size;
   int yes=1;

   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   
   host_addr.sin_family = AF_INET;         // host byte order
   host_addr.sin_port = htons(31337);      // short, network byte order
   host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
   memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

   bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));

   listen(sockfd, 4);

   sin_size = sizeof(struct sockaddr_in);
   new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
}
