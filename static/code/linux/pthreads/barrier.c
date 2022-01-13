#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 9
#define COUNT       7

pthread_barrier_t barrier;

void *start(void *arg) {
  long i = (long) arg;

  while (true) {
    printf("[%ld] waiting...\n", i);
    sleep(1);

    pthread_barrier_wait(&barrier);

    printf("[%ld] done!\n", i);
    sleep(1);
  }
}

pthread_barrier_t barrier;

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  pthread_barrier_init(&barrier, NULL, COUNT);

  for (long i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, &start, (void *) i + 1);
  }

  for (long i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_barrier_destroy(&barrier);
}
