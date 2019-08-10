#include <unistd.h>

int main() {
  char filename[] = "/bin/sh\x00";
  char **argv, **envp; // arrays that contain char pointers

  argv[0] = filename; // only argument is filename
  argv[1] = 0;  // null terminate the argument array

  envp[0] = 0; // null terminate the environment array

  execve(filename, argv, envp);
}
