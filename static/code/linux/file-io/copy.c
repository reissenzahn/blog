#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  
  if (argc != 3) {
    fprintf(stderr, "usage: %s src dest\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int input_fd = open(argv[1], O_RDONLY);
  if (input_fd == -1) {
    fprintf(stderr, "open: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // rw-rw-rw-
  int output_fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (output_fd == -1) {
    fprintf(stderr, "open: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  char buf[BUFSIZE];
  ssize_t num_read;
  while ((num_read = read(input_fd, buf, BUFSIZE)) > 0) {
    if (write(output_fd, buf, num_read) != num_read) {
      fprintf(stderr, "write: failed to write whole buffer\n");
      exit(EXIT_FAILURE);
    }
  }

  if (num_read == -1) {
    fprintf(stderr, "read: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (close(input_fd) == -1) {
    fprintf(stderr, "close: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (close(output_fd) == -1) {
    fprintf(stderr, "close: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}