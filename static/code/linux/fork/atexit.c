#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void handler1(void) {
  printf("exit function 1 called\n");
}

void handler2(void) {
  printf("exit function 2 called\n");
}

int main(int argc, char *argv[]) {

  printf("ATEXIT_MAX = %ld\n", sysconf(_SC_ATEXIT_MAX));

  int rc = atexit(handler1);
  if (rc != 0) {
    fprintf(stderr, "atexit: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }

  rc = atexit(handler2);
  if (rc != 0) {
    fprintf(stderr, "atexit: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}