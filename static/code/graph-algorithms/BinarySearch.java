
public class BinarySearch {

  public static int search(int[] arr, int t) {
    int l = 0;
    int r = arr.length - 1;

    while (l <= r) {
      int m = (l + r) / 2;  // integer division truncates towards 0

      if (arr[m] < t) {
        l = m + 1;
      } else if (arr[m] > t) {
        r = m - 1;
      } else {
        return m;
      }
    }

    return -1; 
  }

  public static void main(String[] args) {
    System.out.println(BinarySearch.search(new int[]{7, 14, 19, 27, 31, 38, 45, 56, 63, 72}, 56));
  }
}
