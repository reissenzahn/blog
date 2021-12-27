


// a mutex guarantees that only one thread accesses a shared variable at a time

// a mutex is like a token that is passed from one thread to another, allowing one thread at a time to proceed

// in order for a thread to access a shared variable, it has to “get” the mutex; when it is done, it “releases” the mutex; only one thread can hold the mutex at a time

// we create a semaphore named mutex that is initialized to 1; a value of one means that a thread may proceed and access the shared variable; a value of zero means that it has to wait for another thread to release the mutex


public class Mutex {

  public static void main(String[] args) {

    int count = 0;

    new Thread(() -> {
      count++;
    }).start();

    new Thread(() -> {
      count++;
    }).start();

    System.out.println(count);
  }
}


// Thread A
// count = count + 1 

// Thread B
// count = count + 1 

// Since mutex is initially 1, whichever thread gets to the wait first will be able to proceed immediately. Of course, the act of waiting on the semaphore has the effect of decrementing it, so the second thread to arrive will have to wait until the first signals. I have indented the update operation to show that it is contained within the mutex. In this example, both threads are running the same code. This is sometimes called a symmetric solution. If the threads have to run different code, the solu-tion is asymmetric. Symmetric solutions are often easier to generalize. In this case, the mutex solution can handle any number of concurrent threads without modification. As long as every thread waits before performing an update and signals after, then no two threads will access count concurrently. Often the code that needs to be protected is called the critical section, I suppose because it is critically important to prevent concurrent access.
