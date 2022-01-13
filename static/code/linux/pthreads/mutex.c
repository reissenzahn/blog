#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 100
#define LIMIT 100000

long count;
pthread_mutex_t mutex;

void *start(void *arg) {

  for (int i = 0; i < LIMIT; i++) {

    // the pthread_mutex_lock() function acquires the specified mutex; if the mutex is already locked then this call will block until the mutex is unlocked
    pthread_mutex_lock(&mutex);

    // pthread_mutex_trylock() will attempt to acquire a mutex; if the mutex is already locked then this call will return immediately with a "busy" error code

    count++;
  
    // the pthread_mutex_unlock() function releases a mutex
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  // a mutex can either by initialized statically with PTHREAD_MUTEX_INITIALIZER or dynamically with pthread_mutex_init(); the mutex is initially unlocked
  pthread_mutex_init(&mutex, NULL);

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

  printf("count: %ld\n", count);
}
