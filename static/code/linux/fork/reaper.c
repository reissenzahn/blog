#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define COUNT 5

volatile int numLiveChildren = 0;

void handle_sigchld(int sig) {
  int saved_errno = errno;

  // NOTE: use of non-async-signal-safe function printf() is unsafe
  printf("caught SIGCHILD\n");

  pid_t pid;
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    printf("reaped pid = %ld\n", (long) pid);
    numLiveChildren--;
  }

  if (pid == -1 && errno != ECHILD) {
    fprintf(stderr, "waitpid: %s\n", strerror(errno));
  }

  // artificially lengthen call to handler to see that multiple SIGCHLD signals are not queued while the handler is already invoked
  sleep(3);

  printf("handler returned\n");

  errno = saved_errno;
}

int main(int argc, char *argv[]) {

  // disable buffering of stdout
  setbuf(stdout, NULL);

  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_sigchld;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    fprintf(stderr, "sigaction: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // block SIGCHLD to prevent its delivery if a child terminates before the parent commences the sigsuspend() loop below
  sigset_t block_mask;
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGCHLD);

  if (sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1) {
    fprintf(stderr, "sigprocmask: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < COUNT; i++) {
    switch(fork()) {
      case -1:
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
      case 0:
        sleep(3);
        printf("child %d (pid=%ld) terminating\n", i, (long) getpid());
        _exit(EXIT_SUCCESS);
      default:
        break;
    }
  }

  numLiveChildren = COUNT;

  sigset_t empty_mask;
  sigemptyset(&empty_mask);

  int count = 0;

  // If we failed to block SIGCHLD in this way, and a child terminated between the test of the value of numLiveChildren and the execution of the sigsuspend() call (or alternatively a pause() call), then the sigsuspend() call would block forever waiting for a signal that has already been caught.
  while (numLiveChildren > 0) {
    if (sigsuspend(&empty_mask) == -1 && errno != EINTR) {
      fprintf(stderr, "sigsuspend: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    count++;
  }

  printf("SIGCLD was caught %d times\n", count);

  exit(EXIT_SUCCESS);
}