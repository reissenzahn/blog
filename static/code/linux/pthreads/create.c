#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void *start(void *arg) {
  pthread_t tid;

  int *limit = (int *) arg;

  // the pthread_self() function returns the thread id of the calling thread
  tid = pthread_self();

  for (int i = 0; i < *limit; i++) {
    printf("thread %lu: %d\n", (unsigned long) tid, i);
  }

  // a thread terminates when it returns from its starting routine or calls the pthread_exit() function
  pthread_exit(NULL);
}

// the pthread_create() function creates a new thread with the given starting routine
int main(int argc, char *argv[]) {
  pthread_t threads[5];
  int limits[5] = {5, 5, 5, 5, 5};

  for (int i = 0; i < 5; i++) {
    int rc = pthread_create(&threads[i], NULL, start, (void *) &limits[i]);

    if (rc != 0) {
      fprintf(stderr, "pthread_create: %s", strerror(rc));
      exit(EXIT_FAILURE);
    }
  }

  sleep(1);

  exit(EXIT_SUCCESS);
}