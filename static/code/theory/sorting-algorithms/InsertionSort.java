// status: WIP
// source: ?

import java.util.*;

public class InsertionSort {

  public static void sort(int[] array) {
    if (array == null) {
      return;
    }

    for (int i = 1; i < array.length; i++) {
      for (int j = i; j > 0 && array[j] < array[j - 1]; j--) {
        swap(array, j - 1, j);
      }
    }
  }

  private static void swap(int[] array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }

  public static void main(String[] args) {
    int[] array = {6, 4, 2, 8, 9, 1, 5, 3, 7};
    InsertionSort.sort(array);
    System.out.println(Arrays.toString(array));  // [1, 2, 3, 4, 5, 6, 7, 8, 9]
  }
}