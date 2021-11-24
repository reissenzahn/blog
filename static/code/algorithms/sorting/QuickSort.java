// status: WIP
// source: https://github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/sorting/QuickSort.java

import java.util.*;

public class QuickSort {

  public static void sort(int[] array) {
    if (array == null) return;

    sort(array, 0, array.length - 1);
  }

  private static void sort(int[] array, int lo, int hi) {
    System.out.println(Arrays.toString(array));

    // base case is an array of length zero or one
    if (hi <= lo) return;
    
    int pivotIndex = partition(array, lo, hi);
    sort(array, lo, pivotIndex - 1);
    sort(array, pivotIndex + 1, hi);
  }

  private static int partition(int[] array, int lo, int hi) {
    int pivot = array[lo];
    
    int l = lo + 1;  // we have chosen array[lo] as the pivot
    int r = hi;  

    while (true) {
      // move l right until it reaches an element >= pivot
      while (array[l] < pivot) {
        if (l == hi) break;
        l++;
      }

      // move r left until it reaches an element <= pivot
      while (array[r] > pivot) {
        if (r == lo) break;
        r--;
      }

      // check if pointers cross
      if (l >= r) break;

      swap(array, l, r);
    }

    swap(array, lo, r);

    // return pivot index
    return r;
  }

  private static void swap(int[] array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;

    System.out.println(Arrays.toString(array));
  }

  public static void main(String[] args) {
    int[] array = {10, 4, 6, 4, 8, -13, 2, 3};
    QuickSort.sort(array);
    System.out.println(Arrays.toString(array));  // [-13, 2, 3, 4, 4, 6, 8, 10]
  }
}