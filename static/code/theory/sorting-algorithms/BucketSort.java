import java.util.*;

public class BucketSort {

  public static void sort(int[] array) {
    int min = array[0];
    int max = array[0];

    for (int i = 0; i < array.length; i++) {
      if (array[i] < min) min = array[i];
      if (array[i] > max) max = array[i];
    }

    // initialized buckets
    int numBuckets = max - min + 1;

    List<List<Integer>> buckets = new ArrayList<>(numBuckets);

    for (int i = 0; i < numBuckets; i++) {
      buckets.add(new ArrayList<>());
    }

    // distribute array values among buckets
    for (int value : array) {
      int index = (value - min) / numBuckets;
      buckets.get(index).add(value);
    }

    // sort each bucket
    for (List<Integer> bucket : buckets) {
      Collections.sort(bucket);
    }

    // concatenate buckets
    int i = 0;
    for (List<Integer> bucket : buckets) {
      for (int value : bucket) {
        array[i] = value;
        i++;
      }
    }
  }

  // javac BucketSort.java && java BucketSort
  public static void main(String[] args) {
    int[] array = new int[1000];

    Random random = new Random();
    for (int i = 0; i < array.length; ++i) {
      array[i] = random.nextInt(10000) - 50;
    }

    BucketSort.sort(array);

    for (int i = 0; i < array.length - 1; i++) {
      assert array[i] <= array[i + 1];
    }
  }
}
