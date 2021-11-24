public class MergeSort {

  public static void sort(int[] array) {
    sort(array, new int[array.length], 0, array.length - 1);
  }

  private static void sort(int array[], int[] aux, int lo, int hi) {
    if (hi &lt;= lo) {
      return;
    }

    int mid = lo + (hi - lo) / 2;

    sort(array, aux, lo, mid);
    sort(array, aux, mid + 1, hi);

    merge(array, aux, lo, mid, hi);
  }

  private static void merge(int[] array, int[] aux, int lo, int mid, int hi) {
    for (int k = lo; k &lt;= hi; k++) {
      aux[k] = array[k];
    }

    int i = lo;
    int j = mid + 1;

    for (int k = lo; k &lt;= hi; k++) {
      if (i &gt; mid) {
        array[k] = aux[j];
        j++;
      } else if (j &gt; hi) {
        array[k] = aux[i];
        i++;
      } else if (aux[j] &lt; aux[i]) {
        array[k] = aux[j];
        j++;
      } else {
        array[k] = aux[i];
        i++;
      }
    }
  }
}