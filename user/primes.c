#include "kernel/types.h"
#include "user/user.h"

int  fwrite(int fd, void* data, int size);
void printPrimes(int fd);

int main() {
  int rightPipe[2];
  pipe(rightPipe);

  int forkRes = fork();

  if (forkRes == 0) {
    close(rightPipe[1]);
    printPrimes(rightPipe[0]);
    exit(0);
  }
  else if (forkRes > 0) {
    close(rightPipe[0]);
    for (int i = 2; i <= 35; i++) {
      fwrite(rightPipe[1], &i, 4);
    }
    int endFlag = 0;
    fwrite(rightPipe[1], &endFlag, 4);
    close(rightPipe[1]);
    wait(0);
    exit(0);
  }
  else {
    fprintf(2, "fork failed\n");
    exit(1);
  }
}

// 传入一个左侧管道的读取文件描述符，从中读取数据，然后打印出来，将合适的数据送入右侧的管道
void printPrimes(int fd) {
  int firstNum;
  read(fd, &firstNum, 4);
  if (firstNum == 0) {
    close(fd);
    exit(0);
  }
  printf("prime %d\n", firstNum);

  int rightPipe[2];
  pipe(rightPipe);
  int forkRes = fork();

  if (forkRes == 0) {
    close(rightPipe[1]);
    printPrimes(rightPipe[0]);
    exit(0);
  }
  else if (forkRes > 0) {
    close(rightPipe[0]);
    int num;
    while (1) {
      read(fd, &num, 4);
      if (num == 0) {
        break;
      }
      if (num % firstNum != 0) {
        fwrite(rightPipe[1], &num, 4);
      }
    }
    int endFlag = 0;
    fwrite(rightPipe[1], &endFlag, 4);
    close(rightPipe[1]);
    wait(0);
    exit(0);
  }
  else {
    fprintf(2, "fork failed\n");
    exit(1);
  }
}

int fwrite(int fd, void* data, int size) {
  int readSize;
  if ((readSize = write(fd, data, size)) != size) {
    fprintf(2, "fwrite error\n");
    exit(1);
  }
  return readSize;
}
