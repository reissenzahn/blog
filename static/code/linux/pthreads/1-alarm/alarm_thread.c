#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct alarm_cmd {
  int sec;
  char msg[64];
};

void *alarm_thread(void *arg) {
  struct alarm_cmd *cmd = (struct alarm_cmd *) arg;

  int s;

  s = pthread_detach(pthread_self());
  if (s != 0) {
    perror("pthread_detach");
    exit(EXIT_FAILURE);
  }

  sleep(cmd->sec);

  printf("(%d) %s\n", cmd->sec, cmd->msg);

  free(cmd);

  return NULL;
}

int main(int argc, char *argv[]) {
  int s;
  char line[128];
  struct alarm_cmd *cmd;
  pthread_t thread;

  while (1) {
    printf("alarm> ");

    if (fgets(line, sizeof(line), stdin) == NULL) {
      exit(EXIT_SUCCESS);
    }

    if (strlen(line) <= 1) {
      continue;
    }

    cmd = (struct alarm_cmd *) malloc(sizeof(struct alarm_cmd));
    if (cmd == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    if (sscanf(line, "%d %64[^\n]", &cmd->sec, cmd->msg) < 2) {
      fprintf(stderr, "invalid command\n");
      free(cmd);
      continue;
    }

    s = pthread_create(&thread, NULL, alarm_thread, cmd);
    if (s != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }
}


