#include <pthread.h>

static void * threadFunc(void *arg)
{
 char *s = (char *) arg;
 printf("%s", s);
 return (void *) strlen(s);
}

int main(int argc, char *argv[])
{
 pthread_t t1;
 void *res;
 int s;
 s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
 if (s != 0)
 errExitEN(s, "pthread_create");
 printf("Message from main()\n");
 s = pthread_join(t1, &res);
 if (s != 0)
 errExitEN(s, "pthread_join");
 printf("Thread returned %ld\n", (long) res);
 exit(EXIT_SUCCESS);
}

---

pthread_mutex_t mtx;
pthread_mutexattr_t mtxAttr;
int s, type;
s = pthread_mutexattr_init(&mtxAttr);
if (s != 0)
errExitEN(s, "pthread_mutexattr_init");
s = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK);
if (s != 0)
errExitEN(s, "pthread_mutexattr_settype");
s = pthread_mutex_init(mtx, &mtxAttr);
if (s != 0)
errExitEN(s, "pthread_mutex_init");
s = pthread_mutexattr_destroy(&mtxAttr); /* No longer needed */
if (s != 0)
errExitEN(s, "pthread_mutexattr_destroy");

---

#include <pthread.h>

static int glob = 0;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg)
{
 int loops = *((int *) arg);
 int loc, j, s;
 for (j = 0; j < loops; j++) {
 s = pthread_mutex_lock(&mtx);
 if (s != 0)
 errExitEN(s, "pthread_mutex_lock");
 loc = glob;
 loc++;
 glob = loc;
 s = pthread_mutex_unlock(&mtx);
 if (s != 0)
 errExitEN(s, "pthread_mutex_unlock");
 }
 return NULL;
}

int main(int argc, char *argv[])
{
 pthread_t t1, t2;
 int loops, s;
 loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
 s = pthread_create(&t1, NULL, threadFunc, &loops);
 if (s != 0)
 errExitEN(s, "pthread_create");
 s = pthread_create(&t2, NULL, threadFunc, &loops);
 if (s != 0)
 errExitEN(s, "pthread_create");
 s = pthread_join(t1, NULL);
 if (s != 0)
 errExitEN(s, "pthread_join");
 s = pthread_join(t2, NULL);
 if (s != 0)
 errExitEN(s, "pthread_join");
 printf("glob = %d\n", glob);
 exit(EXIT_SUCCESS);
}

---

#include <pthread.h>

static void *
threadFunc(void *arg)
{
 int j;
Threads: Thread Cancellation 675
 printf("New thread started\n"); /* May be a cancellation point */
 for (j = 1; ; j++) {
 printf("Loop %d\n", j); /* May be a cancellation point */
 sleep(1); /* A cancellation point */
 }
 /* NOTREACHED */
 return NULL;
}
int
main(int argc, char *argv[])
{
 pthread_t thr;
 int s;
 void *res;
 s = pthread_create(&thr, NULL, threadFunc, NULL);
 if (s != 0)
 errExitEN(s, "pthread_create");
 sleep(3); /* Allow new thread to run a while */
 s = pthread_cancel(thr);
 if (s != 0)
 errExitEN(s, "pthread_cancel");
 s = pthread_join(thr, &res);
 if (s != 0)
 errExitEN(s, "pthread_join");
 if (res == PTHREAD_CANCELED)
 printf("Thread was canceled\n");
 else
 printf("Thread was not canceled (should not happen!)\n");
 exit(EXIT_SUCCESS);
}

---

#include <pthread.h>
#include "tlpi_hdr.h"
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0; /* Predicate variable */
static void /* Free memory pointed to by 'arg' and unlock mutex */
cleanupHandler(void *arg)
{
 int s;
 printf("cleanup: freeing block at %p\n", arg);
q free(arg);
 printf("cleanup: unlocking mutex\n");
w s = pthread_mutex_unlock(&mtx);
 if (s != 0)
 errExitEN(s, "pthread_mutex_unlock");
}
static void *
threadFunc(void *arg)
{
 int s;
 void *buf = NULL; /* Buffer allocated by thread */
e buf = malloc(0x10000); /* Not a cancellation point */
 printf("thread: allocated memory at %p\n", buf);
r s = pthread_mutex_lock(&mtx); /* Not a cancellation point */
 if (s != 0)
 errExitEN(s, "pthread_mutex_lock");
t pthread_cleanup_push(cleanupHandler, buf);
 while (glob == 0) {
y s = pthread_cond_wait(&cond, &mtx); /* A cancellation point */
 if (s != 0)
 errExitEN(s, "pthread_cond_wait");
 }
 printf("thread: condition wait loop completed\n");
u pthread_cleanup_pop(1); /* Executes cleanup handler */
 return NULL;
}
int
main(int argc, char *argv[])
{
 pthread_t thr;
 void *res;
 int s;
Threads: Thread Cancellation 679
i s = pthread_create(&thr, NULL, threadFunc, NULL);
 if (s != 0)
 errExitEN(s, "pthread_create");
 sleep(2); /* Give thread a chance to get started */
 if (argc == 1) { /* Cancel thread */
 printf("main: about to cancel thread\n");
o s = pthread_cancel(thr);
 if (s != 0)
 errExitEN(s, "pthread_cancel");
 } else { /* Signal condition variable */
 printf("main: about to signal condition variable\n");
 glob = 1;
a s = pthread_cond_signal(&cond);
 if (s != 0)
 errExitEN(s, "pthread_cond_signal");
 }
s s = pthread_join(thr, &res);
 if (s != 0)
 errExitEN(s, "pthread_join");
 if (res == PTHREAD_CANCELED)
 printf("main: thread was canceled\n");
 else
 printf("main: thread terminated normally\n");
 exit(EXIT_SUCCESS);
}
