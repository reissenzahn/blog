public class BinarySearch {

  // equality
  public static int search(int[] array, int target) {
    int lo = 0;
    int hi = array.length - 1;

    while (lo <= hi) {
      int mid = (lo + hi) / 2;

      if (array[mid] == target) {
        return mid;
      } else if (array[mid] < target) {
        lo = mid + 1;
      } else {  // data[mid] > target
        hi = mid - 1;
      }
    }

    return -1;
  }

  // greatest element less than or equal
  public static int searchClosest(int[] array, int target) {
    int lo = 0;
    int hi = array.length - 1;

    while (lo < hi) {
      int mid = (lo + hi) / 2;

      if (array[mid] < target) {
        lo = mid;
      } else {
        hi = mid - 1;
      }
    }

    return lo;
  }

  public static void main(String[] args) {
    int[] array = {7, 14, 19, 27, 31, 38, 45, 56, 63, 72};

    System.out.println(BinarySearch.search(array, 45));  // 6

    System.out.println(BinarySearch.searchClosest(array, 46));  // 6
  }
}
