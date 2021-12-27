import java.util.concurrent.Semaphore;

// There is a table with five plates, five chopsticks and a big bowl of rice. Five philosophers alternate between thinking and eating. The philosophers need two chopsticks to eat.

// We need to ensure that only one philosopher can hold a chopstick though we want it to be possible for more than one philosopher to eat at the same time so as to maximize the amount of concurrency.

// Further, we need to prevent deadlock as well as starvation of a philosopher waiting for a chopstick.


// 1. 


class DiningPhilosophers {

  public static void attempt1() {

    Semaphore[] chopsticks = new Semaphore[5];

    for (int i = 0; i < 5; i++) {
      chopsticks[i] = new Semaphore(1);
    }

    for 
  }

  private static int left(int i) {
    return i;
  }

  private static int right(int i) {
    return (i + 1) % 5;
  }

  public static void main(String[] args) {

  }
}


