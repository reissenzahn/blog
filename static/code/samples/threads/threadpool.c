#include <stdbool.h>
#include <stddef.h>


// The two functions of importance are tpool_add_work which adds work to the queue for processing and tpool_wait which blocks until all work has been completed.
bool tpool_add_work(tpool_t *tm, thread_func_t func, void *arg);
void tpool_wait(tpool_t *tm);

struct tpool_work {
    thread_func_t      func;
    void              *arg;
    struct tpool_work *next;
};
typedef struct tpool_work tpool_work_t;

// The work queue is a simple linked list which stores the function to call and its arguments. A result queue could be implemented in the same way. However, I prefer to have the results handled by the work function because it gives a bit more flexibility. For example, not all work needs a result and not all results need thread synchronization.

struct tpool {
    tpool_work_t    *work_first;
    tpool_work_t    *work_last;
    pthread_mutex_t  work_mutex;
    pthread_cond_t   work_cond;
    pthread_cond_t   working_cond;
    size_t           working_cnt;
    size_t           thread_cnt;
    bool             stop;
};


Since the work queue is implemented as a linked list work_first and work_last are used to push and pop work objects. There is a single mutex (work_mutex) which is used for all locking.

We need two conditionals, work_cond signals the threads that there is work to be processed and working_cond signals when there are no threads processing. We combine these with working_cnt to know how many threads are actively processing work. We’re not checking if the threads running because they’ll always be running as part of the pool. They’re just not using resources if they’re not processing any work.

Finally, we track how many threads are alive using thread_cnt. This helps us prevent running threads from being destroyed prematurely. This is used in conjunction with stop which actually stops the threads.

static tpool_work_t *tpool_work_create(thread_func_t func, void *arg)
{
    tpool_work_t *work;

    if (func == NULL)
        return NULL;

    work       = malloc(sizeof(*work));
    work->func = func;
    work->arg  = arg;
    work->next = NULL;
    return work;
}

static void tpool_work_destroy(tpool_work_t *work)
{
    if (work == NULL)
        return;
    free(work);
}
Simple helpers for creating and destroying work objects.


static tpool_work_t *tpool_work_get(tpool_t *tm)
{
    tpool_work_t *work;

    if (tm == NULL)
        return NULL;

    work = tm->work_first;
    if (work == NULL)
        return NULL;

    if (work->next == NULL) {
        tm->work_first = NULL;
        tm->work_last  = NULL;
    } else {
        tm->work_first = work->next;
    }

    return work;
}

Work will need to be pulled from the queue at some point to be processed. Since the queue is a linked list this handles not only pulling an object from the list but also maintaining the list work_first and work_last references for us.

The Worker Function
static void *tpool_worker(void *arg)
{
    tpool_t      *tm = arg;
    tpool_work_t *work;

    while (1) {
        pthread_mutex_lock(&(tm->work_mutex));

        while (tm->work_first == NULL && !tm->stop)
            pthread_cond_wait(&(tm->work_cond), &(tm->work_mutex));

        if (tm->stop)
            break;

        work = tpool_work_get(tm);
        tm->working_cnt++;
        pthread_mutex_unlock(&(tm->work_mutex));

        if (work != NULL) {
            work->func(work->arg);
            tpool_work_destroy(work);
        }

        pthread_mutex_lock(&(tm->work_mutex));
        tm->working_cnt--;
        if (!tm->stop && tm->working_cnt == 0 && tm->work_first == NULL)
            pthread_cond_signal(&(tm->working_cond));
        pthread_mutex_unlock(&(tm->work_mutex));
    }

    tm->thread_cnt--;
    pthread_cond_signal(&(tm->working_cond));
    pthread_mutex_unlock(&(tm->work_mutex));
    return NULL;
}
This is the heart and soul of the pool and is where work is handled. At a high level this waits for work and processes it.

    while (1) {
This will keep the tread running and as long as it doesn’t exit it can be used.

        pthread_mutex_lock(&(tm->work_mutex));
The first thing that happens is locking the mutex so we can be sure nothing else manipulates the pool’s members.

        while (tm->work_first == NULL && !tm->stop)
            pthread_cond_wait(&(tm->work_cond), &(tm->work_mutex));
Check if there is any work available for processing and we are still running. We’ll wait in a conditional until we’re signaled and run our check again. Remember the conditional automatically unlocks the mutex so others can acquire the lock. When the conditional is signaled it will relock the mutex automatically so we don’t have to. We’re looping here instead of using an if statement to handle spurious wakeups.

        if (tm->stop)
            break;
Now we check if the pool has requested that all threads stop running and exit. We want to keep holding the lock so we can modify a few things outside of the run loop. The mutex will be unlocked before the thread exists. The stop check is all the way up here because we want to stop before pulling any work.

        work = tpool_work_get(tm);
        tm->working_cnt++;
        pthread_mutex_unlock(&(tm->work_mutex));
Once the thread was signaled there is work, we’ll pull some from the queue and increment working_cnt so the pool knows a thread is processing. The mutex is unlocked so other threads can pull and process work. We want the work processing to happen in parallel it doesn’t make sense to hold the lock while work is processing. The lock is only there to synchronize pulling work from the queue.

        if (work != NULL) {
            work->func(work->arg);
            tpool_work_destroy(work);
        }
If there was work, process it and destroy the work object. It is possible that there was no work at this point so there isn’t anything that needs to be done. For example, lets say there is one piece of work and 4 threads. All threads are signaled there is work. Each one will unblock one at a time and pull the work, so the first thread will acquire the lock pull the work, lock and start processing. The next three will unblock and pull nothing from the queue because it’s empty.

        pthread_mutex_lock(&(tm->work_mutex));
        tm->working_cnt--;
        if (!tm->stop && tm->working_cnt == 0 && tm->work_first == NULL)
            pthread_cond_signal(&(tm->working_cond));
        pthread_mutex_unlock(&(tm->work_mutex));
Finally, once the work has been processed (or not if there wasn’t any) the mutex will be locked again and working_cnt is decreased because the work is done. If there are no threads working and there are no items in the queue a signal will be sent to inform the wait function (in case someone’s waiting) to wake up.

Since we know that only one thread can hold the lock, then only the last thread to finish it’s work can decrement working_cnt to zero. This way we know there aren’t any threads working.

    tm->thread_cnt--;
    pthread_cond_signal(&(tm->working_cond));
    pthread_mutex_unlock(&(tm->work_mutex));
    return NULL;
At the top of the run loop we break out on stop and this is where it leads us. We decrement the thread count because this thread is stopping.

Then we signal tpool_wait that a thread has exited. We need to do this here because tpool_wait will wait for all threads to exit when stopping. If we’re here it’s because tpool_destroy was called and it’s waiting for tpool_wait to exit. Once all threads have exited tpool_wait will return allowing tpool_destroy to finish.

We unlock the mutex last because everything is protected by it, and we’re guaranteed tpool_wait won’t wake up before the thread finishes it’s cleanup. The signal above only runs after we unlock.

Pool create and destroy
tpool_t *tpool_create(size_t num)
{
    tpool_t   *tm;
    pthread_t  thread;
    size_t     i;

    if (num == 0)
        num = 2;

    tm             = calloc(1, sizeof(*tm));
    tm->thread_cnt = num;

    pthread_mutex_init(&(tm->work_mutex), NULL);
    pthread_cond_init(&(tm->work_cond), NULL);
    pthread_cond_init(&(tm->working_cond), NULL);

    tm->work_first = NULL;
    tm->work_last  = NULL;

    for (i=0; i<num; i++) {
        pthread_create(&thread, NULL, tpool_worker, tm);
        pthread_detach(thread);
    }

    return tm;
}
When creating the pool the default is two threads if zero was specified. Otherwise, the caller specified number will be used. It’s a good idea for us to have a default but two might be a little low. This goes back to the idea of using the number of core/processors + 1 as the default. If you’re using the my thread wrapper it has a cross platform function, pcthread_get_num_procs, that will give this to you.

    for (i=0; i<num; i++) {
        pthread_create(&thread, NULL, tpool_worker, tm);
        pthread_detach(thread);
    }
The requested number of threads are started and tpool_worker is specified as the thread function. The threads are detached so they will cleanup on exit. There is no need to store the thread ids because they will never be accessed directly. If we wanted to implement some kind of force exit instead of having to wait then we’d need to track the ids.

void tpool_destroy(tpool_t *tm)
{
    tpool_work_t *work;
    tpool_work_t *work2;

    if (tm == NULL)
        return;

    pthread_mutex_lock(&(tm->work_mutex));
    work = tm->work_first;
    while (work != NULL) {
        work2 = work->next;
        tpool_work_destroy(work);
        work = work2;
    }
    tm->stop = true;
    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    tpool_wait(tm);

    pthread_mutex_destroy(&(tm->work_mutex));
    pthread_cond_destroy(&(tm->work_cond));
    pthread_cond_destroy(&(tm->working_cond));

    free(tm);
}
Once all outstanding processing finishes the pool object is destroyed.

    pthread_mutex_lock(&(tm->work_mutex));
    work = tm->work_first;
    while (work != NULL) {
        work2 = work->next;
        tpool_work_destroy(work);
        work = work2;
    }
We are throwing away all pending work but the caller really should have dealt with this situation since they’re the one that called destroy. Typically, destroy will only be called when all work is done and nothing is processing. However, it’s possible someone is trying to force processing to stop. In which case there will be work queued and we need to get clean it up.

This only empties the queue and since it’s in the work_mutex we have full access to the queue. This won’t interfere with anything currently processing because the threads have pulled off the work they’re working on. Any threads trying to pull new work will we’ve finished clearing out any pending work.

    tm->stop = true;
    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));
Once we’ve cleaned up the queue we’ll tell the threads they need to stop.

    tpool_wait(tm);
Some threads may have already been running and are currently processing so we need to wait for them to finish. This is where a force exit parameter could be implemented which would kill all threads instead of waiting on them to finish but this is a bad idea. For example, if you’re writing to a database you could corrupt it.

Even though we could add force quit now and never use it we probably don’t want to have it at all because people might not realize they shouldn’t use it. Wait, why would we even implement something that we won’t use and we don’t want people to use? Maybe that’s why we didn’t do it in the first place.

Adding work to the queue
bool tpool_add_work(tpool_t *tm, thread_func_t func, void *arg)
{
    tpool_work_t *work;

    if (tm == NULL)
        return false;

    work = tpool_work_create(func, arg);
    if (work == NULL)
        return false;

    pthread_mutex_lock(&(tm->work_mutex));
    if (tm->work_first == NULL) {
        tm->work_first = work;
        tm->work_last  = tm->work_first;
    } else {
        tm->work_last->next = work;
        tm->work_last       = work;
    }

    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    return true;
}
Adding to the work queue consists of creating a work object, locking the mutex and adding the object to the liked list.

Waiting for processing to complete
void tpool_wait(tpool_t *tm)
{
    if (tm == NULL)
        return;

    pthread_mutex_lock(&(tm->work_mutex));
    while (1) {
        if ((!tm->stop && tm->working_cnt != 0) || (tm->stop && tm->thread_cnt != 0)) {
            pthread_cond_wait(&(tm->working_cond), &(tm->work_mutex));
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&(tm->work_mutex));
}
This is a blocking function that will only return when there is no work. The mutex is locked and we we wait in a conditional if there are any threads processing, if there is still work to do or if the threads are stopping and not all have exited. The retry is a safety measure in case of spurious wake ups. Once there is nothing processing return so the caller can continue.

Testing
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "tpool.h"

static const size_t num_threads = 4;
static const size_t num_items   = 100;

void worker(void *arg)
{
    int *val = arg;
    int  old = *val;

    *val += 1000;
    printf("tid=%p, old=%d, val=%d\n", pthread_self(), old, *val);

    if (*val%2)
        usleep(100000);
}

int main(int argc, char **argv)
{
    tpool_t *tm;
    int     *vals;
    size_t   i;

    tm   = tpool_create(num_threads);
    vals = calloc(num_items, sizeof(*vals));

    for (i=0; i<num_items; i++) {
        vals[i] = i;
        tpool_add_work(tm, worker, vals+i);
    }

    tpool_wait(tm);

    for (i=0; i<num_items; i++) {
        printf("%d\n", vals[i]);
    }

    free(vals);
    tpool_destroy(tm);
    return 0;
}

---

C - Simple Thread Pool Based on pthread Implementation
What is a thread pool? My simple understanding is that there is a set of pre-derived threads, and then an administrator to manage and schedule these threads, you just need to constantly give him the task to complete, he will schedule the resources of the thread to help you complete.

So what does the administrator do? A simple way is for an administrator to manage a task queue and add tasks to the end of the queue if new tasks are received. Each thread stares at the queue. If the queue is not empty, it takes a task to process at the head of the queue (each task can only be taken by one thread). After processing, it goes on to the queue to fetch the task. If there are no tasks, the thread will sleep until the task queue is not empty. If the administrator is smarter, he may reduce the number of threads when there are no tasks or few tasks, and increase the number of threads when tasks can not be handled later, thus realizing the dynamic management of resources.

So what is the task? In the background server, for example, every user's request is a task. Threads constantly take requests out of the request queue and continue to process the next request after completion.

The simple illustration is as follows:

One advantage of thread pooling is that it reduces the time of thread creation and destruction. This benefit is very significant when the task processing time is relatively short, and it can improve the efficiency of task processing.

Thread pool implementation
threadpool_task_t
Used to save a task waiting to be executed. A task needs to be specified: the corresponding function to be run and the parameters of the function. So there are function pointers and void pointers in the struct.

typedef struct {
    void (*function)(void *);
    void *argument;
} threadpool_task_t;
thread_pool_t
The structure of a thread pool. Because it is C language, the task queue here is implemented by using arrays and maintaining the head and tail of the queue.

struct threadpool_t {
  pthread_mutex_t lock;     /* mutex */
  pthread_cond_t notify;    /* Conditional variable */
  pthread_t *threads;       /* Starting Pointer of Thread Array */
  threadpool_task_t *queue; /* Starting Pointer of Task Queue Array */
  int thread_count;         /* Number of threads */
  int queue_size;           /* Task queue length */
  int head;                 /* Current task queue head */
  int tail;                 /* End of current task queue */
  int count;                /* Number of tasks currently to be run */
  int shutdown;             /* Is the current state of the thread pool closed? */
  int started;              /* Number of threads running */
};
function
External interface
threadpool_t *threadpool_create(int thread_count, int queue_size, int flags); create a thread pool, use thread_count to specify the number of derived threads, queue_size to specify the length of task queue, flags as reserved parameters, unused.
int threadpool_add(threadpool_t *pool, void (*routine)(void *),void *arg, int flags); add tasks to be performed. The second parameter is the corresponding function pointer, and the third is the corresponding function parameter. Flags is not in use.
Int threadpool_destroy (threadpool_t*pool, int flags); destroy the existing thread pool. Flags can specify whether to end immediately or peacefully. Immediate termination refers to the immediate termination of a task queue regardless of whether it is empty or not. Peaceful ending refers to waiting for tasks in the task queue to complete after the completion of the task, in this process can not add new tasks.
Internal auxiliary function
static void *threadpool_thread(void *threadpool); functions executed by each thread in the thread pool.
Int threadpool_free (threadpool_t*pool); releases the memory resources requested by the thread pool.
Thread pool usage
Compile
Refer to Makefile in the project root directory and compile it directly with make.

test case
The project provides three test cases (see threadpool/test/), which we can use to learn the use of thread pools and test whether they work properly. Here is one of them:

#define THREAD 32
#define QUEUE  256

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "threadpool.h"

int tasks = 0, done = 0;
pthread_mutex_t lock;

void dummy_task(void *arg) {
    usleep(10000);
    pthread_mutex_lock(&lock);
    /* Record the number of successful tasks completed */
    done++;
    pthread_mutex_unlock(&lock);
}

int main(int argc, char **argv)
{
    threadpool_t *pool;

    /* Initialize mutex */
    pthread_mutex_init(&lock, NULL);

    /* Assertion Thread Pool Creation Successful */
    assert((pool = threadpool_create(THREAD, QUEUE, 0)) != NULL);
    fprintf(stderr, "Pool started with %d threads and "
            "queue size of %d\n", THREAD, QUEUE);

    /* As long as the task queue is not full, add it all the time */
    while(threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
        pthread_mutex_lock(&lock);
        tasks++;
        pthread_mutex_unlock(&lock);
    }

    fprintf(stderr, "Added %d tasks\n", tasks);

    /* Continuously check whether the number of tasks is more than half completed, and if not continue to sleep. */
    while((tasks / 2) > done) {
        usleep(10000);
    }
    /* At this point, destroy the thread pool, 0 for immediate_shutdown */
    assert(threadpool_destroy(pool, 0) == 0);
    fprintf(stderr, "Did %d tasks\n", done);

    return 0;
}
Source Code Annotations
Source comments are put together in github. Click on me.

threadpool.h
/*
 * Copyright (c) 2013, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#ifdef __cplusplus
/* For C++ compilers, specify C's syntax compilation */
extern "C" {
#endif

/**
 * @file threadpool.h
 * @brief Threadpool Header File
 */

 /**
 * Increase this constants at your own risk
 * Large values might slow down your system
 */
#define MAX_THREADS 64
#define MAX_QUEUE 65536

/* Simplified variable definition */
typedef struct threadpool_t threadpool_t;

/* Define error codes */
typedef enum {
    threadpool_invalid        = -1,
    threadpool_lock_failure   = -2,
    threadpool_queue_full     = -3,
    threadpool_shutdown       = -4,
    threadpool_thread_failure = -5
} threadpool_error_t;

typedef enum {
    threadpool_graceful       = 1
} threadpool_destroy_flags_t;

/* Here are three external API s for thread pool */

/**
 * @function threadpool_create
 * @brief Creates a threadpool_t object.
 * @param thread_count Number of worker threads.
 * @param queue_size   Size of the queue.
 * @param flags        Unused parameter.
 * @return a newly created thread pool or NULL
 */
/**
 * Create a thread pool with thread_count threads and queue_size task queues. The flags parameter is not used
 */
threadpool_t *threadpool_create(int thread_count, int queue_size, int flags);

/**
 * @function threadpool_add
 * @brief add a new task in the queue of a thread pool
 * @param pool     Thread pool to which add the task.
 * @param function Pointer to the function that will perform the task.
 * @param argument Argument to be passed to the function.
 * @param flags    Unused parameter.
 * @return 0 if all goes well, negative values in case of error (@see
 * threadpool_error_t for codes).
 */
/**
 *  Add tasks to thread pool, pool is thread pool pointer, routine is function pointer, arg is function parameter, flags is not used
 */
int threadpool_add(threadpool_t *pool, void (*routine)(void *),
                   void *arg, int flags);

/**
 * @function threadpool_destroy
 * @brief Stops and destroys a thread pool.
 * @param pool  Thread pool to destroy.
 * @param flags Flags for shutdown
 *
 * Known values for flags are 0 (default) and threadpool_graceful in
 * which case the thread pool doesn't accept any new tasks but
 * processes all pending tasks before shutdown.
 */
/**
 * Destroy thread pools, flags can be used to specify how to close them
 */
int threadpool_destroy(threadpool_t *pool, int flags);

#ifdef __cplusplus
}
#endif

#endif /* _THREADPOOL_H_ */
threadpool.c
/*
 * Copyright (c) 2013, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file threadpool.c
 * @brief Threadpool implementation file
 */

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "threadpool.h"

/**
 * How thread pool closes
 */
typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown  = 2
} threadpool_shutdown_t;

/**
 *  @struct threadpool_task
 *  @brief the work struct
 *
 *  @var function Pointer to the function that will perform the task.
 *  @var argument Argument to be passed to the function.
 */
/**
 * Definition of a task in thread pool
 */

typedef struct {
    void (*function)(void *);
    void *argument;
} threadpool_task_t;

/**
 *  @struct threadpool
 *  @brief The threadpool struct
 *
 *  @var notify       Condition variable to notify worker threads.
 *  @var threads      Array containing worker threads ID.
 *  @var thread_count Number of threads
 *  @var queue        Array containing the task queue.
 *  @var queue_size   Size of the task queue.
 *  @var head         Index of the first element.
 *  @var tail         Index of the next element.
 *  @var count        Number of pending tasks
 *  @var shutdown     Flag indicating if the pool is shutting down
 *  @var started      Number of started threads
 */
/**
 * Structural Definition of Thread Pool
 *  @var lock         Mutex Locks for Internal Work
 *  @var notify       Conditional variables for interthread notifications
 *  @var threads      Thread array, represented here by pointer, array name = first element pointer
 *  @var thread_count Number of threads
 *  @var queue        An array of stored tasks, namely task queues
 *  @var queue_size   Task queue size
 *  @var head         The first task position in the task queue (Note: All tasks in the task queue are not running)
 *  @var tail         Next location of the last task in the task queue (note: queues are stored in arrays, and head and tail indicate queue location)
 *  @var count        The number of tasks in the task queue, that is, the number of tasks waiting to run
 *  @var shutdown     Indicates whether the thread pool is closed
 *  @var started      Number of threads started
 */
struct threadpool_t {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *threads;
  threadpool_task_t *queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;
  int started;
};

/**
 * @function void *threadpool_thread(void *threadpool)
 * @brief the worker thread
 * @param threadpool the pool which own the thread
 */
/**
 * Functions in the thread pool where each thread is running
 * Declare that static should only be used to make functions valid in this file
 */
static void *threadpool_thread(void *threadpool);

int threadpool_free(threadpool_t *pool);

threadpool_t *threadpool_create(int thread_count, int queue_size, int flags)
{
    if(thread_count <= 0 || thread_count > MAX_THREADS || queue_size <= 0 || queue_size > MAX_QUEUE) {
        return NULL;
    }

    threadpool_t *pool;
    int i;

    /* Request memory to create memory pool objects */
    if((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL) {
        goto err;
    }

    /* Initialize */
    pool->thread_count = 0;
    pool->queue_size = queue_size;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = pool->started = 0;

    /* Allocate thread and task queue */
    /* Memory required to request thread arrays and task queues */
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    pool->queue = (threadpool_task_t *)malloc
        (sizeof(threadpool_task_t) * queue_size);

    /* Initialize mutex and conditional variable first */
    /* Initialize mutexes and conditional variables */
    if((pthread_mutex_init(&(pool->lock), NULL) != 0) ||
       (pthread_cond_init(&(pool->notify), NULL) != 0) ||
       (pool->threads == NULL) ||
       (pool->queue == NULL)) {
        goto err;
    }

    /* Start worker threads */
    /* Create a specified number of threads to start running */
    for(i = 0; i < thread_count; i++) {
        if(pthread_create(&(pool->threads[i]), NULL,
                          threadpool_thread, (void*)pool) != 0) {
            threadpool_destroy(pool, 0);
            return NULL;
        }
        pool->thread_count++;
        pool->started++;
    }

    return pool;

 err:
    if(pool) {
        threadpool_free(pool);
    }
    return NULL;
}

int threadpool_add(threadpool_t *pool, void (*function)(void *),
                   void *argument, int flags)
{
    int err = 0;
    int next;

    if(pool == NULL || function == NULL) {
        return threadpool_invalid;
    }

    /* Mutual exclusion lock ownership must be acquired first */
    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return threadpool_lock_failure;
    }

    /* Calculate the next location where task can be stored */
    next = pool->tail + 1;
    next = (next == pool->queue_size) ? 0 : next;

    do {
        /* Are we full ? */
        /* Check if the task queue is full */
        if(pool->count == pool->queue_size) {
            err = threadpool_queue_full;
            break;
        }

        /* Are we shutting down ? */
        /* Check whether the current thread pool state is closed */
        if(pool->shutdown) {
            err = threadpool_shutdown;
            break;
        }

        /* Add task to queue */
        /* Place function pointers and parameters in tail to add to the task queue */
        pool->queue[pool->tail].function = function;
        pool->queue[pool->tail].argument = argument;
        /* Update tail and count */
        pool->tail = next;
        pool->count += 1;

        /* pthread_cond_broadcast */
        /*
         * Send out signal to indicate that task has been added
         * If a thread is blocked because the task queue is empty, there will be a wake-up call.
         * If not, do nothing.
         */
        if(pthread_cond_signal(&(pool->notify)) != 0) {
            err = threadpool_lock_failure;
            break;
        }
        /*
         * The do {...} while (0) structure is used here.
         * Ensure that the process is executed at most once, but in the middle it is easy to jump out of the execution block because of exceptions
         */
    } while(0);

    /* Release mutex resources */
    if(pthread_mutex_unlock(&pool->lock) != 0) {
        err = threadpool_lock_failure;
    }

    return err;
}

int threadpool_destroy(threadpool_t *pool, int flags)
{
    int i, err = 0;

    if(pool == NULL) {
        return threadpool_invalid;
    }

    /* Get mutex resources */
    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return threadpool_lock_failure;
    }

    do {
        /* Already shutting down */
        /* Determine whether it has been closed elsewhere */
        if(pool->shutdown) {
            err = threadpool_shutdown;
            break;
        }

        /* Gets the specified closing mode */
        pool->shutdown = (flags & threadpool_graceful) ?
            graceful_shutdown : immediate_shutdown;

        /* Wake up all worker threads */
        /* Wake up all threads blocked by dependent variables and release mutexes */
        if((pthread_cond_broadcast(&(pool->notify)) != 0) ||
           (pthread_mutex_unlock(&(pool->lock)) != 0)) {
            err = threadpool_lock_failure;
            break;
        }

        /* Join all worker thread */
        /* Waiting for all threads to end */
        for(i = 0; i < pool->thread_count; i++) {
            if(pthread_join(pool->threads[i], NULL) != 0) {
                err = threadpool_thread_failure;
            }
        }
        /* Also do{...} while(0) structure*/
    } while(0);

    /* Only if everything went well do we deallocate the pool */
    if(!err) {
        /* Release memory resources */
        threadpool_free(pool);
    }
    return err;
}

int threadpool_free(threadpool_t *pool)
{
    if(pool == NULL || pool->started > 0) {
        return -1;
    }

    /* Did we manage to allocate ? */
    /* Release memory resources occupied by thread task queue mutex conditional variable thread pool */
    if(pool->threads) {
        free(pool->threads);
        free(pool->queue);

        /* Because we allocate pool->threads after initializing the
           mutex and condition variable, we're sure they're
           initialized. Let's lock the mutex just in case. */
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->notify));
    }
    free(pool);
    return 0;
}


static void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;

    for(;;) {
        /* Lock must be taken to wait on conditional variable */
        /* Get mutex resources */
        pthread_mutex_lock(&(pool->lock));

        /* Wait on condition variable, check for spurious wakeups.
           When returning from pthread_cond_wait(), we own the lock. */
        /* The purpose of using while is to re-examine the condition during wake-up. */
        while((pool->count == 0) && (!pool->shutdown)) {
            /* The task queue is empty and the thread pool is blocked when it is not closed */
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        /* Closing Processing */
        if((pool->shutdown == immediate_shutdown) ||
           ((pool->shutdown == graceful_shutdown) &&
            (pool->count == 0))) {
            break;
        }

        /* Grab our task */
        /* Get the first task in the task queue */
        task.function = pool->queue[pool->head].function;
        task.argument = pool->queue[pool->head].argument;
        /* Update head and count */
        pool->head += 1;
        pool->head = (pool->head == pool->queue_size) ? 0 : pool->head;
        pool->count -= 1;

        /* Unlock */
        /* Release mutex */
        pthread_mutex_unlock(&(pool->lock));

        /* Get to work */
        /* Start running tasks */
        (*(task.function))(task.argument);
        /* Here is the end of a task run */
    }

    /* Threads will end, update the number of running threads */
    pool->started--;

    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return(NULL);
}
Posted by om.bitsian on Fri, 19 Jul 2019 23:06:20 -0700