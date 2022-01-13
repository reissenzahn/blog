#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 5
#define TARGET 17

void *start(void *arg) {
  pthread_t tid = pthread_self();

  printf("%lu: starting...\n", (unsigned long) tid);

  long ret = 1;
  for (int i = 1; i <= TARGET; i++) {
    ret *= i;
  }
    
  printf("%lu: finished!\n", (unsigned long) tid);

  pthread_exit((void *) ret);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  // only threads that are created as joinable can be joined; if a thread is created as detached then it can never be joined
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (int i = 0; i < NUM_THREADS; i++) {
    int rc = pthread_create(&threads[i], &attr, start, NULL);
    if (rc != 0) {
      fprintf(stderr, "pthread_create: %s", strerror(rc));
      exit(EXIT_FAILURE);
    }
  }

  pthread_attr_destroy(&attr);

  for (int i = 0; i < NUM_THREADS; i++) {
    // the pthread_join() function blocks the calling thread until the specified thread terminates
    void *status;
    int rc = pthread_join(threads[i], &status);
    if (rc != 0) {
      fprintf(stderr, "pthread_join: %s", strerror(rc));
      exit(EXIT_FAILURE);
    }

    // we can obtain the return status of the threads when using pthread_join()
    printf("main: %ld\n", (long) status);
  }

  exit(EXIT_SUCCESS);
}

// the pthread_detach() routine can be used to explicitly detach a thread even though it was created as joinable