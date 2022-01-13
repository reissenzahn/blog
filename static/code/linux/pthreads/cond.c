#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

// we consider the case of a service station with fuel reserves that are resupplied by a tanker and consumed by cars

#define NUM_CONSUMERS 10
#define NUM_PRODUCERS 3

#define CONSUME 40
#define PRODUCE 60
#define PRODUCE_STEPS 5

int available = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *consumer(void *arg) {
  long i = (long) arg;

  pthread_mutex_lock(&mutex);

  while (available < CONSUME) {
    printf("[consumer %d] waiting (available=%d)\n", i, available);

    pthread_cond_wait(&cond, &mutex);
  }

  available -= CONSUME;

  printf("[consumer %d] consumed %d (available=%d)\n", i, CONSUME, available);

  pthread_mutex_unlock(&mutex);
}

void *producer(void *arg) {
  for (int i = 0; i < PRODUCE_STEPS; i++) {
    pthread_mutex_lock(&mutex);
    
    available += PRODUCE;
    
    printf("[producer] produced %d (available=%d)\n", PRODUCE, available);
    
    pthread_mutex_unlock(&mutex);
    
    // pthread_cond_signal(&cond);
    pthread_cond_broadcast(&cond);
    
    sleep(1);
  }
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_CONSUMERS + NUM_PRODUCERS];

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  for (long i = 0; i < NUM_CONSUMERS; i++) {
    pthread_create(&threads[i], NULL, &consumer, (void *) i);
  }

  sleep(1);

  for (long i = NUM_CONSUMERS - 1; i < NUM_CONSUMERS + NUM_PRODUCERS; i++) {
    pthread_create(&threads[i], NULL, &producer, (void *) i);
  }

  for (int i = 0; i < NUM_CONSUMERS + NUM_PRODUCERS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  exit(EXIT_SUCCESS);
}
