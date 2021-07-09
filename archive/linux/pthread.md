

```
pthread_t thread;

int pthread_equal(pthread_t t1, pthread_t t2);

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);

pthread_t pthread_self(void);

int sched_yield(void);

int pthread_exit(void *value_ptr);

int pthread_detach(pthread_t thread);

int pthread_join(pthread_t thread, void **value_ptr);
```

Within your program a thread is represented by a thread identifier of the opaque type pthread_t.

To create a thread, you must declare a variable of type pthread_t.

A pthreads thread begins by calling some function that you provide.

This function should expect a single argument of type void * and should return a value of the same type.

You create a thread by passing the thread function's address and the argument value with which you want the function to be called to pthread_create.

When you create a thread, pthread_create returns an identifier, in the pthread_t value referred to by the thread argument, by which your code refers to the new thread.

A thread can also get its own identifier using the pthread_self function.

Pthreads provides the pthread_equal function to compare two thread identifiers.

The pthread_equal function returns a nonzero value if the thread identifiers refer to the same thread, and the value 0 if they do not refer to the same thread.

When a C program runs, it begins in a special function named main. In a threaded program, this special stream of execution is called the “initial thread” or sometimes the “main thread.”

The initial thread is special because when the function main returns the process terminates without allowing other threads to complete.

Detaching a thread that is still running doesn't affect the thread in any
way—it just informs the system th at the thread's resources can be
reclaim ed when the thread eventually terminates.
I
Although “thread evaporation” is sometimes useful, most of the time your pro-


