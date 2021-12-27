
class HelloThread extends Thread {

  private int limit;

  public Thread(int limit) {
    this.limit = limit;
  }

  public void run() {
    for (int i = 0; i < limit; i++) {
      System.out.println(i);
    }
  }
}

class HelloRunner implements Runnable {

  public void run() {
    System.out.println("Hello!");
  }
}


public class CreatingThreads {

  public static void main(String[] args) {
    
    // extending Thread
    HelloThread thread = new HelloThread();
    thread.start();

    // anonymous subclass of Thread
    Thread thread = new Thread(){
      public void run(){
        System.out.println("Hello!");
      }
    }
    thread.start();

    // anonymous implementation of Runnable
    Runnable runnable = new Runnable(){
      public void run() {
        System.out.println("Hello!");
      }
    }


    Runnable runnable = () -> {
      System.out.println("Hello!");
    }

    Thread thread = new Thread(runnable);
    thread.start();

  }
}

// 
