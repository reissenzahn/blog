#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

long getfilesize1(char *filename) {
  struct stat status;
  if (stat(filename, &status) == -1) {
    fprintf(stderr, "stat(): %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  return status.st_size;
}

long getfilesize2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }
  
  if (fseek(fp, 0, SEEK_END) == -1) {
    fprintf(stderr, "fseek(): %s\n", strerror(errno));
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  long len = ftell(fp);
  fclose(fp);
  return len;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *filename = argv[1];

  printf("%ld\n", getfilesize1(filename));
  printf("%ld\n", getfilesize2(filename));

  return EXIT_SUCCESS;
}
