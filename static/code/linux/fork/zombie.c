#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <libgen.h>

#define CMD_SIZE 200

int main(int argc, char *argv[]) {

  // disable buffering of stdout
  setbuf(stdout, NULL);

  printf("parent pid = %ld\n", (long) getpid());

  pid_t pid = fork();
  
  switch (pid) {
  case -1:
    fprintf(stderr, "fork: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  case 0:
    printf("child pid = %ld\n", (long) getpid());

    // become zombie immediately
    _exit(EXIT_SUCCESS);
  default:
    sleep(3);

    char cmd[CMD_SIZE];
    snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
    cmd[CMD_SIZE - 1] = '\0';
    system(cmd);

    // try send kill signal to the zombie
    if (kill(pid, SIGKILL) == -1) {
      fprintf(stderr, "kill: %s\n", strerror(errno));
    }

    sleep(3);

    system(cmd);

    exit(EXIT_SUCCESS);
  }
}