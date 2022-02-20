public class MergeSort {

  public static void sort(int[] array) {
    sort(array, new int[array.length], 0, array.length - 1);
  }

  private static void sort(int array[], int[] aux, int lo, int hi) {
    if (hi <= lo) {
      return;
    }

    int mid = lo + (hi - lo) / 2;

    sort(array, aux, lo, mid);
    sort(array, aux, mid + 1, hi);

    merge(array, aux, lo, mid, hi);
  }

  private static void merge(int[] array, int[] aux, int lo, int mid, int hi) {
    for (int k = lo; k <= hi; k++) {
      aux[k] = array[k];
    }

    int i = lo;
    int j = mid + 1;

    for (int k = lo; k <= hi; k++) {
      if (i > mid) {
        array[k] = aux[j];
        j++;
      } else if (j > hi) {
        array[k] = aux[i];
        i++;
      } else if (aux[j] < aux[i]) {
        array[k] = aux[j];
        j++;
      } else {
        array[k] = aux[i];
        i++;
      }
    }
  }
}