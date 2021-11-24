import java.util.*;

public class Fibonacci {

  public int recursive(int n) {
    if (n < 2) return n;

    return this.recursive(n - 1) + this.recursive(n - 2);
  }

  public int topDown(Map<Integer, Integer> map, int n) {
    if (map == null) {
      map = new HashMap<>();
      map.put(0, 0);
      map.put(1, 1);
    }

    if (map.containsKey(n)) return map.get(n);

    int m = this.topDown(map, n - 1) + this.topDown(map, n - 2);

    map.put(n, m);

    return m;
  }

  public int bottomUp(int n) {
    int[] arr = new int[n + 1];

    arr[0] = 0;
    arr[1] = 1;

    for (int i = 2; i <= n; i++) {
      arr[i] = arr[i - 1] + arr[i - 2];
    }

    return arr[n];
  }

  public int bottomUp2(int n) {
    int prev = 0;
    int curr = 1;

    for (int i = 0; i < n - 1; i++) {
      int next = prev + curr;
      prev = curr;
      curr = next;
    }

    return curr;
  }

  public static void main(String[] args) {
    Fibonacci fib = new Fibonacci();

    System.out.println(fib.recursive(8));
    System.out.println(fib.topDown(null, 8));
    System.out.println(fib.bottomUp(8));
    System.out.println(fib.bottomUp2(8));
  }
}