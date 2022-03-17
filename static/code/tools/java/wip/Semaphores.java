

public class Semaphores {
  
  public static void main(String[] args) {
    
  }
}


ExecutorService executor = Executors.newFixedThreadPool(10);

Semaphore semaphore = new Semaphore(5);

Runnable longRunningTask = () -> {
    boolean permit = false;
    try {
        permit = semaphore.tryAcquire(1, TimeUnit.SECONDS);
        if (permit) {
            System.out.println("Semaphore acquired");
            sleep(5);
        } else {
            System.out.println("Could not acquire semaphore");
        }
    } catch (InterruptedException e) {
        throw new IllegalStateException(e);
    } finally {
        if (permit) {
            semaphore.release();
        }
    }
}

IntStream.range(0, 10)
    .forEach(i -> executor.submit(longRunningTask));

stop(executor);

It’s important to use a try/finally block to properly release the semaphore even in case of exceptions.

The semaphores permits access to the actual long running operation simulated by sleep(5) up to a maximum of 5. Every subsequent call to tryAcquire() elapses the maximum wait timeout of one second, resulting in the appropriate console output that no semaphore could be acquired.


