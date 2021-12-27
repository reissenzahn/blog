import java.util.*;

public class RadixSort {

  public static void sort(int[] array) {
    
    // find maximum value
    int max = array[0];
    for (int i = 0; i < array.length; i++) {
      if (array[i] > max) {
        max = array[i];
      }
    }

    // find number of digits in maximum value
    int numDigits = ((int) Math.log10(max)) + 1;

    // start at the LSD    
    int place = 1;

    while (numDigits > 0) {
      countingSort(array, place);
      numDigits--;
      place *= 10;
    }
  }

  private static void countingSort(int[] array, int place) {
    int range = 10;

    int[] counts = new int[range];
    int[] sorted = new int[array.length];

    for (int i = 0; i < array.length; i++) {
      int digit = (array[i] / place) % range;
      counts[digit]++;
    }

    for (int i = 1; i < range; i++) {
      counts[i] += counts[i - 1];
    }

    for (int i = array.length - 1; i >= 0; i--) {
      int digit = (array[i] / place) % range;
      sorted[counts[digit] - 1] = array[i];
      counts[digit]--;
    }

    for (int i = 0; i < array.length; i++) {
      array[i] = sorted[i];
    }
  }

  // javac RadixSort.java && java RadixSort
  public static void main(String[] args) {
    int[] array = new int[1000];

    Random random = new Random();
    for (int i = 0; i < array.length; ++i) {
      array[i] = random.nextInt(10000);
    }

    RadixSort.sort(array);

    for (int i = 0; i < array.length - 1; i++) {
      assert array[i] <= array[i + 1];
    }
  }
}