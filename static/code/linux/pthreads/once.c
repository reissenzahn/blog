#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

// the behavior of pthread_once() is undefined if once_control  has automatic storage duration or is not initialized by PTHREAD_ONCE_INIT
pthread_once_t control = PTHREAD_ONCE_INIT;

void init() {
  printf("initializing...\n");
}

void *start(void *arg) {

  // the pthread_once() function calls a given function only once regardless of the number of times it is called
  int rc = pthread_once(&control, init);
  if (rc != 0) {
    fprintf(stderr, "pthread_once: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

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
    int rc = pthread_join(threads[i], NULL);
    if (rc != 0) {
      fprintf(stderr, "pthread_join: %s", strerror(rc));
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
