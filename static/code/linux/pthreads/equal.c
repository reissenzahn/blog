#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

pthread_t self;

void *start(void *arg) {
  self = pthread_self();
  
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t thread;

  int rc = pthread_create(&thread, NULL, start, NULL);
  if (rc != 0) {
    fprintf(stderr, "pthread_create: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }

  sleep(1);

  // the pthread_equal() function compares two threads to see if they are equal
  printf(pthread_equal(thread, self) ? "true" : "false");

  exit(EXIT_SUCCESS);
}