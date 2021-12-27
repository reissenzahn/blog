import java.util.Arrays;

public class CountingSort {

  public static void sort(int array[]) {
    int n = array.length;

    // find the largest element in the array
    int max = array[0];
    for (int i = 1; i < n; i++) {
      if (array[i] > max)
        max = array[i];
    }

    int[] count = new int[max + 1];

    for (int i = 0; i < max; i++) {
      count[i] = 0;
    }

    // determine the number of occurrences of each element
    for (int i = 0; i < n; i++) {
      count[array[i]]++;
    }

    // compute prefix sum
    for (int i = 1; i <= max; i++) {
      count[i] += count[i - 1];
    }

    // 
    int[] output = new int[n];

    for (int i = n - 1; i >= 0; i--) {
      output[count[array[i]] - 1] = array[i];
      count[array[i]]--;
    }

    // copy the sorted array into original array
    for (int i = 0; i < n; i++) {
      array[i] = output[i];
    }
  }

  public static void main(String args[]) {
    int[] array = {4, 2, 2, 8, 3, 3, 1};

    CountingSort.sort(array);

    System.out.println(Arrays.toString(array));
  }
}
