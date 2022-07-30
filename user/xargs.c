#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int getWord(int fd, char* buf);
int getSentence(int fd, char* buf);

int main(int argc, char* argv[]) {
  while (1) {
    char   buf[512];
    char** cmd = (char**)malloc(sizeof(char*) * argc);
    int    top = 0;
    for (int i = 0; i < argc; i++) {
      cmd[i] = (char*)malloc(sizeof(char) * 64);
    }
    memset(buf, 0, 512);

    if (getSentence(0, buf) == 0) {
      break;
    }
    for (int i = 0; i < argc - 1; i++) {
      strcpy(cmd[top++], argv[i + 1]);
    }
    strcpy(cmd[top++], buf);
    int forkRes = fork();
    if (forkRes == 0) {
      exec(cmd[0], cmd);
      fprintf(2, "exec %s failed\n", cmd[0]);
      exit(1);
    }
    else if (forkRes > 0) {
      for (int i = 0; i < argc; i++) {
        free(cmd[i]);
      }
      free(cmd);
      wait(0);
    }
    else {
      fprintf(2, "fork error\n");
      exit(1);
    }
  }

  exit(0);
}

int getSentence(int fd, char* buf) {
  char* p = buf;
  char  c = 0;

  while (read(fd, &c, 1) > 0) {
    if (c == '\n') {
      return 1;
    }
    *p++ = c;
  }

  return 0;
}
