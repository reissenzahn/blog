#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main() {
  char template[] = "/tmp/file-XXXXXX";

  int fd = mkstemp(template);
  if (fd == -1) {
    fprintf(stderr, "mkstemp: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
 
  printf("%s\n", template);

  // name disappears immediately but the file is removed only after close()
  unlink(template);

  char msg[] = "hello";
  if (write(fd, msg, 6) == -1) {
    fprintf(stderr, "write: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (lseek(fd, 0, SEEK_SET) == -1) {
    fprintf(stderr, "lseek: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  char buf[6];
  if (read(fd, buf, 6) == -1) {
    fprintf(stderr, "read: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("%s\n", buf);

  if (close(fd) == -1) {
    fprintf(stderr, "close: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}