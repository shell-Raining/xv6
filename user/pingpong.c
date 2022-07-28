#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p[2];
  pipe(p);
  int  forkRes = fork();
  char byte;

  if (forkRes == 0) {
    read(p[0], &byte, 1);
    close(p[0]);
    printf("%d: received ping\n", getpid());
    if (write(p[1], &byte, 1) != 1) {
      fprintf(2, "parent: write failed\n");
      exit(1);
    }
    close(p[1]);
    exit(0);
  }
  else if (forkRes > 0) {
    byte = 'a';
    if (write(p[1], &byte, 1) != 1) {
      fprintf(2, "parent: write failed\n");
      exit(1);
    }
    close(p[1]);
    wait(0);
    read(p[0], &byte, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());
    exit(0);
  }
  else {
    fprintf(2, "fork failed\n");
    exit(1);
  }
}
