#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define COUNT 5

int main(int argc, char *argv[]) {

  // disable buffering of stdout
  setbuf(stdout, NULL);

  for (int i = 0; i < COUNT; i++) {
    switch (fork()) {
    case -1:
      fprintf(stderr, "fork: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    case 0:
      sleep(3);
      _exit(EXIT_SUCCESS);
    default:
      break;
    }
  }

  // wait for each child to exit
  while (true) {
    pid_t pid = wait(NULL);

    if (pid == -1) {
      if (errno == ECHILD) {
        printf("bye!\n");
        exit(EXIT_SUCCESS);
      }

      fprintf(stderr, "wait: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    printf("pid = %ld\n", (long) pid);
  }
}