public class Loops {

  // $ javac Loops.java && java Loops
  public static void main(String[] args) {
  
    // for
    for (int i = 0; i < 5; i++) {
      System.out.print(i + " ");  // 0 1 2 3 4
    }
    System.out.println();

    for (int i = 0, j = 5; i < 5; i++, j--) {
      System.out.print((i + j) + " ");  // 5 5 5 5 5
    }
    System.out.println();

    // foreach
    int[] arr = new int[] {2, 3, 5, 7, 11};
    for (int i : arr) {
      System.out.print(i + " ");  // 2 3 5 7 11
    }
    System.out.println();


    // while
    int n = 0;
    while (n < 5) {
      System.out.print(n + " ");  // 0 1 2 3 4
      n++;
    }
    System.out.println();

    // do-while
    int m = 0;
    do {
      System.out.println(m);  // 0
    } while (m > 0);


    // break terminates the current loop
    int p = 0;
    while (true) {
      if (p == 5) break;
      System.out.print(p + " ");  // 0 1 2 3 4
      p++;
    }
    System.out.println();

    // continue causes a loop to immediately go to the next iteration
    for (int i = 0; i < 10; i++) {
      if (i % 2 != 0) continue;
      System.out.print(i + " ");  // 0 2 4 6 8
    }
    System.out.println();

    // bail out of nested loops
    while (true) {
      while (true) {
        break bail;
      }
    }
    bail:
  }
}