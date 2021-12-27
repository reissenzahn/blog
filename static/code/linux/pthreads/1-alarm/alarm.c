#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int seconds;
  char line[128];
  char message[64];

  while (1) {
    printf("alarm> ");

    if (fgets(line, sizeof(line), stdin) == NULL) {
      exit(EXIT_SUCCESS);
    }

    if (strlen(line) <= 1) {
      continue;
    }

    if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2) {
      fprintf(stderr, "invalid command\n");
      continue;
    }

    sleep(seconds);

    printf("(%d) %s\n", seconds, message);
  }
}