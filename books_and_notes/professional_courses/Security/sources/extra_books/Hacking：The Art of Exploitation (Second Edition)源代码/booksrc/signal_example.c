#include <stdio.h> 
#include <stdlib.h>
#include <signal.h> 
/* some labeled signal defines from signal.h 
 * #define SIGHUP        1  hangup 
 * #define SIGINT        2  interrupt  (Ctrl-C) 
 * #define SIGQUIT       3  quit (Ctrl-\) 
 * #define SIGILL        4  illegal instruction 
 * #define SIGTRAP       5  trace/breakpoint trap 
 * #define SIGABRT       6  process aborted 
 * #define SIGBUS        7  bus error 
 * #define SIGFPE        8  floating point error 
 * #define SIGKILL       9  kill 
 * #define SIGUSR1      10  user defined signal 1 
 * #define SIGSEGV      11  segmentation fault 
 * #define SIGUSR2      12  user defined signal 2 
 * #define SIGPIPE      13  write to pipe with no one reading 
 * #define SIGALRM      14  countdown alarm set by alarm() 
 * #define SIGTERM      15  termination (sent by kill command) 
 * #define SIGCHLD      17  child process signal 
 * #define SIGCONT      18  continue if stopped 
 * #define SIGSTOP      19  stop (pause execution) 
 * #define SIGTSTP      20  terminal stop [suspend] (Ctrl-Z) 
 * #define SIGTTIN      21  background process trying to read stdin 
 * #define SIGTTOU      22  background process trying to read stdout 
 */ 

/* a signal handler */ 
void signal_handler(int signal) { 
   printf("Caught signal %d\t", signal); 
   if (signal == SIGTSTP) 
      printf("SIGTSTP (Ctrl-Z)"); 
   else if (signal == SIGQUIT) 
      printf("SIGQUIT (Ctrl-\\)"); 
   else if (signal == SIGUSR1) 
      printf("SIGUSR1"); 
   else if (signal == SIGUSR2) 
      printf("SIGUSR2"); 

   printf("\n"); 
} 

void sigint_handler(int x) { 
   printf("Caught a Ctrl-C (SIGINT) in a separate handler\nExiting..\n"); 
   exit(0); 
} 

int main() { 
   /* registering signal handlers */ 
   signal(SIGQUIT, signal_handler); // set signal_handler() as the 
   signal(SIGTSTP, signal_handler); // signal handler for these 
   signal(SIGUSR1, signal_handler); // signals 
   signal(SIGUSR2, signal_handler); 

   signal(SIGINT, sigint_handler);  // set sigint_handler() for SIGINT 

   while(1) {}  // loop forever 
} 
