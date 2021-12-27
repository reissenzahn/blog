#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  // usage check
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "usage: %s [filename]\n", argv[0]);

    // non-zero return values signal unusual or erroneous termination conditions
    return -1;
  }

  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    // print value of errno
    fprintf(stderr, "open: %i\n", errno);

    // print error message
    fprintf(stderr, "open: %s\n", strerror(errno));

    // print error message with prefix
    perror("open");

    return -1;
  }

  close(fd);

  // a return value of zero implies normal termination
  return 0;
}