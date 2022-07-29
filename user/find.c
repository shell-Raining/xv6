#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char* path, char* filename);

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    fprintf(2, "usage: [path] [filename]\n");
    exit(1);
  }

  char* path     = argv[1];
  char* filename = argv[2];

  find(path, filename);

  exit(0);
}

void find(char* path, char* filename) {
  int           fd;
  struct dirent de;
  struct stat   st;
  char          buf[512], *p;

  strcpy(buf, path);
  p    = buf + strlen(buf);
  *p++ = '/';

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
      continue;
    }
    memcpy(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) {
      fprintf(2, "connot stat %s\n", buf);
      exit(1);
    }

    if (st.type == T_FILE && strcmp(de.name, filename) == 0) {
      printf("%s\n", buf);
    }
    if (st.type == T_DIR) {
      find(buf, filename);
    }
  }
  close(fd);
}
