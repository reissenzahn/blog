#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "usage: %s filename {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (fd == -1) {
    fprintf(stderr, "open: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (int i = 2; i < argc; i++) {
    switch (argv[i][0]) {

      // display bytes at current offset
      case 'r':
      case 'R':
        size_t len = atoi(&argv[i][1]);
        
        char *buf = malloc(len);
        if (buf == NULL) {
          fprintf(stderr, "malloc: %s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }

        ssize_t num_read = read(fd, buf, len);
        if (num_read == -1) {
          fprintf(stderr, "read: %s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }

        if (num_read == 0) {
          printf("%s: end-of-file\n", argv[i]);
        } else {
          printf("%s: ", argv[i]);

          for (int j = 0; j < num_read; j++) {
            if (argv[i][0] == 'r') {
              printf("%c", isprint((unsigned char) buf[j]) ? buf[j] : '?');
            } else {
              printf("%02x ", (unsigned int) buf[j]);
            }
          }

          printf("\n");
        }

        free(buf);
        
        break;
      
      // write string at current offset
      case 'w':
        ssize_t num_written = write(fd, &argv[i][1], strlen(&argv[i][1]));
        if (num_written == -1) {
          fprintf(stderr, "write: %s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }

        printf("%s: wrote %ld bytes\n", argv[i], (long) num_written);

        break;
      
      case 's':
        off_t offset = atoi(&argv[i][1]);

        if (lseek(fd, offset, SEEK_SET) == -1) {
          fprintf(stderr, "lseek: %s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }

        printf("%s: seeked to %lld\n", argv[i], (long long) offset);

        break;
      
      default:
        printf("%s: invalid command\n", argv[i]);
    }
  }

  exit(EXIT_SUCCESS);
}

// $ touch test
// $ ./a.out test s100000 wabc
// $ ls -l test Check size of file
// $ ./a.out test s10000 R5