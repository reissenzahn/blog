
static int data = 3;

int main(int argc, char *argv[]) {
  int stack = 4;

  pid_t pid = fork();

  switch(pid) {
  case -1:
    fprintf(stderr, "fork: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  case 0:
    data *= 10;
    stack *= 10;
    break;
  default:
    // give child process a chance to execute
    sleep(3);
    break;
  }

  printf("pid = %ld data = %d stack = %d\n", (long) getpid(), data, stack);

  exit(EXIT_SUCCESS);
}